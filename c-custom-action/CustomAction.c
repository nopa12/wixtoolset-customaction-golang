#include <stdio.h>

#include "stdafx.h"
#include "logging.h"

// disable exports mangling
#ifdef __cplusplus
extern "C"
{
#endif

typedef long int (*cgoCurrentMillis)(void);

HINSTANCE hDLL = NULL;
cgoCurrentMillis goCurrentMillis = NULL;


__declspec(dllexport) UINT __stdcall VerifyCustomAction(MSIHANDLE hInstall)
{
  HRESULT hr = S_OK;
  UINT er = ERROR_SUCCESS;

  // init and parse input args

  log_printf("VerifyCustomAction called\n");

  hr = WcaInitialize(hInstall, "VerifyCustomAction");
  ExitOnFailure(hr, "Failed to initialize");

  log_printf("VerifyCustomAction Initialized.\n");

  hDLL = LoadLibrary("go-dll.dll");
  if (hDLL == NULL) {
      log_printf("Failed to load DLL\n");
      goto LExit;
  }

  goCurrentMillis = (cgoCurrentMillis)GetProcAddress(hDLL, "cgoCurrentMillis");
  if (goCurrentMillis == NULL) {
      log_printf("Failed to load cgoCurrentMillis\n");
      goto LExit;
  }
  
  // parse host
  LPWSTR host = NULL;
  hr = WcaGetProperty(L"HOST", &host);
  ExitOnFailure(hr, "Failure reading HOST");

  log_printf("VerifyCustomAction: HOST = '%ls'\n", host);

LExit:
  if (hDLL) {
    FreeLibrary(hDLL);
  }
  
  // if not, the app crashes
  er = 0;
  log_printf("VerifyCustomAction: end er = '%u'\n", er);
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