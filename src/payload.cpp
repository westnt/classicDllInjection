#include <windows.h>
#include <iostream>

#pragma comment(lib,"user32.lib");

extern "C" __declspec(dllexport) bool APIENTRY DllMain( HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch(reason)
    {
        case DLL_PROCESS_ATTACH:
         MessageBox(0,"process_a", "Woops!", MB_OK);
         break;
         
        case DLL_THREAD_ATTACH:
        //MessageBox(0,"thread_a", "Woops!", MB_OK);
        break;

        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        break;
    }
  return 1;
}
