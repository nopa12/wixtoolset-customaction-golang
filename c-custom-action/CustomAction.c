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

    hDLL = LoadLibrary("go-dll.dll");
    if (hDLL == NULL) {
        log_printf("Failed to load DLL\n");
    }

    goCurrentMillis = (cgoCurrentMillis)GetProcAddress(hDLL, "cgoCurrentMillis");
    if (goCurrentMillis == NULL) {
        log_printf("Failed to load cgoCurrentMillis\n");
    }

    break;

  case DLL_PROCESS_DETACH:
    WcaGlobalFinalize();
    close_log_file();
    FreeLibrary(hDLL);
    break;
  }

  return TRUE;
}

#ifdef __cplusplus
}
#endif