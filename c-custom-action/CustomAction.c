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

HINSTANCE hDLL = NULL;
cgoCurrentMillis goCurrentMillis = NULL;

void UnpackResourceToDisk(void)
{
    HGLOBAL     res_handle = NULL;
    HRSRC       res;
    char *      res_data;
    DWORD       res_size;
    log_printf("Unpacking resource\n");

    HMODULE g_hInstance = GetModuleHandle(NULL);
    log_printf("after GetModuleHandle\n");
    res = FindResource(g_hInstance, MAKEINTRESOURCE(MY_RESOURCE), RT_RCDATA);
    log_printf("after FindResource\n");
    if (!res) {
        log_printf("FindResource failed");
        return;
    }
    res_handle = LoadResource(g_hInstance, res);
    if (!res_handle) {
        log_printf("LoadResource failed");
        return;
    }
    res_data = (char*)LockResource(res_handle);
    res_size = SizeofResource(g_hInstance, res);
    log_printf("Resource size = %d\n", res_size);
    /* you can now use the resource data */
}

__declspec(dllexport) UINT __stdcall CustomActionEntryPoint(MSIHANDLE hInstall)
{
  HRESULT hr = S_OK;
  UINT er = ERROR_SUCCESS;

  // init and parse input args

  log_printf("CustomActionEntryPoint called\n");

  hr = WcaInitialize(hInstall, "CustomActionEntryPoint");
  ExitOnFailure(hr, "Failed to initialize");

  log_printf("CustomActionEntryPoint Initialized.\n");

  // char MyCustomAction[MAX_PATH];
  // DWORD dwSize = MAX_PATH;
  // MsiGetProperty(hInstall, "MyCustomAction", MyCustomAction, &dwSize);

  // log_printf("MyCustomAction path: %s.\n", MyCustomAction);

  UnpackResourceToDisk();

  // hDLL = LoadLibrary("go-dll.dll");
  // if (hDLL == NULL) {
  //     log_printf("Failed to load DLL\n");
  //     goto LExit;
  // }

  // goCurrentMillis = (cgoCurrentMillis)GetProcAddress(hDLL, "cgoCurrentMillis");
  // if (goCurrentMillis == NULL) {
  //     log_printf("Failed to load cgoCurrentMillis\n");
  //     goto LExit;
  // }
  
  // // parse host
  // LPWSTR host = NULL;
  // hr = WcaGetProperty(L"HOST", &host);
  // ExitOnFailure(hr, "Failure reading HOST");

  // log_printf("CustomActionEntryPoint: HOST = '%ls'\n", host);

LExit:
  // if (hDLL) {
  //   FreeLibrary(hDLL);
  // }
  
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