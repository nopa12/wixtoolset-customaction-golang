#include <stdio.h>

#include "stdafx.h"
#include "logging.h"
#include "resource.h"

// disable exports mangling
#ifdef __cplusplus
extern "C"
{
#endif

typedef long int (*cgoCurrentMillis)(void);

#define EXTRACTED_PATH "\\WINDOWS\\temp\\extraced_go_dll.dll"

bool UnpackResourceToDisk(void)
{
    HGLOBAL     res_handle = NULL;
    HRSRC       res = NULL;
    char *      res_data = NULL;
    DWORD       res_size = 0;

    log_printf("Unpacking resource\n");

    HMODULE g_hInstance = NULL;
    GetModuleHandleExA(
          GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
          (LPCTSTR)&UnpackResourceToDisk, // Use any function from the current DLL
          &g_hInstance);

    log_printf("after GetModuleHandle\n");
    res = FindResourceA(g_hInstance, MAKEINTRESOURCEA(IDTHING), RT_RCDATA);
    log_printf("after FindResource\n");
    if (res == NULL) {
        log_printf("FindResource failed\n");
        return false;
    }
    res_handle = LoadResource(g_hInstance, res);
    if (res_handle == NULL) {
        log_printf("LoadResource failed\n");
        return false;
    }
    res_data = (char*)LockResource(res_handle);
    res_size = SizeofResource(g_hInstance, res);
    log_printf("Resource size = %d\n", res_size);

    log_printf("trying to write dll to " EXTRACTED_PATH "\n");

    FILE* outputFile = fopen(EXTRACTED_PATH, "wb");
    if (!outputFile) {
        log_printf("Failed to open " EXTRACTED_PATH " output file for writing.\n");
        return false;
    }
    fwrite(res_data, 1, res_size, outputFile);
    fclose(outputFile);
    log_printf("Resource dll wrote to " EXTRACTED_PATH "\n");
    return true;
}

__declspec(dllexport) UINT __stdcall CustomActionEntryPoint(MSIHANDLE hInstall)
{
  HRESULT hr = S_OK;
  UINT er = ERROR_SUCCESS;
  HINSTANCE hDLL = NULL;
  cgoCurrentMillis goCurrentMillis = NULL;

  // init and parse input args

  log_printf("CustomActionEntryPoint called\n");

  hr = WcaInitialize(hInstall, "CustomActionEntryPoint");
  ExitOnFailure(hr, "Failed to initialize");

  log_printf("CustomActionEntryPoint Initialized.\n");

  if (!UnpackResourceToDisk()) {
    log_printf("Failed to UnpackResourceToDisk\n");
    goto LExit;
  }

  hDLL = LoadLibraryA(EXTRACTED_PATH);
  if (hDLL == NULL) {
      log_printf("Failed to load DLL\n");
      goto LExit;
  }

  goCurrentMillis = (cgoCurrentMillis)GetProcAddress(hDLL, "cgoCurrentMillis");
  if (goCurrentMillis == NULL) {
      log_printf("Failed to load cgoCurrentMillis\n");
      goto LExit;
  }

  long int from_go = goCurrentMillis();

  log_printf("value from goCurrentMillis: %ld\n", from_go);
  
  // // parse host
  // LPWSTR host = NULL;
  // hr = WcaGetProperty(L"HOST", &host);
  // ExitOnFailure(hr, "Failure reading HOST");

LExit:
  if (hDLL) {
    // this is leaked on purpose. there is a deadlock here and better not free it ;)
    // FreeLibrary(hDLL);
  }

  // if not, the app crashes
  er = 0;
  log_printf("CustomActionEntryPoint: end er = '%u'\n", er);
  return WcaFinalize(er);
}


// DllMain - Initialize and cleanup WiX custom action utils.
extern "C" BOOL WINAPI DllMain(
  __in HINSTANCE hInst,
  __in ULONG ulReason,
  __in LPVOID
)
{
  switch (ulReason)
  {
  case DLL_PROCESS_ATTACH:
    WcaGlobalInitialize(hInst);
    open_log_file(LOG_PATH);
    break;

  case DLL_PROCESS_DETACH:
    WcaGlobalFinalize();
    close_log_file();
    break;
  }

  return TRUE;
}

#ifdef __cplusplus
}
#endif