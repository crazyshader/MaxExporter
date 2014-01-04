#include "MaxExporter.h"

HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		hInstance = hinstDLL;            // Hang on to this DLL's instance handle.
		DisableThreadLibraryCalls(hInstance);
	}

	return (TRUE);
}
