#include <windows.h>
#include "../DllTest/dllmain.cpp"

const char* oldStr = "hello";
const char* newStr = "bye";

#define maxCountLength 10
#define maxMessageLength 10000
#define dllPath "../../DllTest\\Debug\\DllTest.dll"
#define methodName "Replace"
#define dllForLoded "kernel32.dll"
#define methodForLoded "LoadLibraryA"
#define BUFFER_SIZE 1024

typedef int(__stdcall* TReplace)(HANDLE, char*, char*, int);

DWORD Inject(DWORD pId, char* dllName)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pId);
    if (!hProcess) return -1;

    LPVOID LibMethod = (LPVOID)GetProcAddress(GetModuleHandleA(dllForLoded), methodForLoded);
    LPVOID str = VirtualAllocEx(hProcess, NULL, strlen(dllName), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hProcess, str, dllName, strlen(dllName), NULL);
    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LibMethod, str, 0, NULL);
    WaitForSingleObject(hRemoteThread, INFINITE);

    DWORD exitCode;
    GetExitCodeThread(hRemoteThread, &exitCode);

    VirtualFreeEx(hProcess, str, strlen(dllName), MEM_RELEASE);
    CloseHandle(hRemoteThread);
    CloseHandle(hProcess);

    return exitCode;
}

int Call(DWORD pid, DWORD dllAdress, char* oldStr, char* newStr, int bufferSize)
{
    HANDLE hZhertva = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!hZhertva) return -1;

    LPVOID str1 = VirtualAllocEx(hZhertva, NULL, strlen(oldStr), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hZhertva, str1, oldStr, strlen(oldStr), NULL);

    LPVOID str2 = VirtualAllocEx(hZhertva, NULL, strlen(newStr), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hZhertva, str2, newStr, strlen(newStr), NULL);

    TArgs args = {(const char*)str1, (const char*)str2, bufferSize};

    LPVOID argsAddr = VirtualAllocEx(hZhertva, NULL, sizeof(TArgs), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hZhertva, argsAddr, &args, sizeof(TArgs), NULL);

    DWORD dllAddr = (DWORD)LoadLibraryA(dllPath);
    DWORD procAddr = (DWORD)GetProcAddress((HMODULE)dllAddr, "Replace2");
    DWORD procOffset = procAddr - dllAddr;
    DWORD procAdress = dllAdress + procOffset;

    HANDLE hRemoteThread = CreateRemoteThread(hZhertva, NULL, NULL, (LPTHREAD_START_ROUTINE)procAdress, argsAddr, 0, NULL);
    if (hRemoteThread == NULL) return -1;

    WaitForSingleObject(hRemoteThread, INFINITE);

    DWORD exitCode;
    GetExitCodeThread(hRemoteThread, &exitCode);

    FreeLibrary((HMODULE)dllAddr);
    CloseHandle(hZhertva);
    CloseHandle(hRemoteThread);

    return (int)exitCode;
}

int main()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    WriteConsoleA(hStdout, "Enter PID: ", 12, NULL, NULL);

    DWORD read;
    char buffer[1024] = { 0 };
    int i = 0;
    while (true)
    {
        ReadConsoleA(hStdin, &(buffer[i]), 1, &read, NULL);
        if (buffer[i] == '\n') break;
        i++;
    }

    DWORD pid = atoi(buffer);

    DWORD dllAdress = Inject(pid, (char*)dllPath);

    int count;
    if (dllAdress == 0) count = -1;
    else count = Call(pid, dllAdress, (char*)oldStr, (char*)newStr, BUFFER_SIZE);
    _itoa_s(count, buffer, 10);

    WriteConsoleA(hStdout, "Replaces: ", 11, NULL, NULL);
    WriteConsoleA(hStdout, buffer, strlen(buffer), NULL, NULL);

    ReadConsoleA(hStdin, buffer, 1, &read, NULL);
}