#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "main.h"

BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			sbapi_main();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
}