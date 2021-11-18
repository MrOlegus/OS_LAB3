#include "pch.h"
#include <Windows.h>
#include <cstdlib>

#define EXPORT __declspec(dllexport)
#define maxAddressValue 0x80000000

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
    return TRUE;
}

int __stdcall ReplaceStr(const char* oldStr, const char* newStr, char* buffer, int bufferSize)
{
    if (strlen(newStr) > strlen(oldStr)) return 0;

    int count = 0;

    for (int i = 0; i <= bufferSize - strlen(oldStr); i++)
    {
        bool isSimilar = true;
        for (int j = 0; j < strlen(oldStr); j++)
            if (oldStr[j] != buffer[i + j]) {
                isSimilar = false;
                break;
            }
        if (isSimilar) {
            count++;
            for (int j = 0; j < strlen(newStr); j++)
                buffer[i + j] = newStr[j];
        }
    }

    return count;
}

EXPORT int __stdcall Replace(HANDLE hProcess, const char* oldStr, const char* newStr, const int bufferSize)
{
    int count = 0;
    char* buffer;
    buffer = (char*)malloc(bufferSize);
    UINT32 adress = 0x00000000;
    while (adress < maxAddressValue)
    {
        MEMORY_BASIC_INFORMATION lpBuffer;
        VirtualQueryEx(hProcess, (LPCVOID)adress, &lpBuffer, sizeof(MEMORY_BASIC_INFORMATION));
        bool res = false;
        int g = 0;
        if ((lpBuffer.Protect & 0xF00) != PAGE_GUARD)
        if (((lpBuffer.Protect & 0xFF) == PAGE_EXECUTE_READ) ||
            ((lpBuffer.Protect & 0xFF) == PAGE_EXECUTE_READWRITE) ||
            ((lpBuffer.Protect & 0xFF) == PAGE_READONLY) ||
            ((lpBuffer.Protect & 0xFF) == PAGE_READWRITE))
              res = ReadProcessMemory(hProcess, (LPCVOID)adress, buffer, bufferSize, NULL);
        if (res)
        {
            int newCount = ReplaceStr(oldStr, newStr, buffer, bufferSize);
            if (newCount > 0)
            {
                if (((lpBuffer.Protect & 0xFF) == PAGE_EXECUTE_READWRITE) ||
                    ((lpBuffer.Protect & 0xFF) == PAGE_EXECUTE_WRITECOPY) ||
                    ((lpBuffer.Protect & 0xFF) == PAGE_READWRITE) ||
                    ((lpBuffer.Protect & 0xFF) == PAGE_WRITECOPY))
                {
                    bool res = false;
                    while (!res) res = WriteProcessMemory(hProcess, (LPVOID)adress, buffer, bufferSize, NULL);
                    count += newCount;
                }
            }
        }
        adress += bufferSize;
    }

    return count;
}

EXPORT struct TArgs
{
    const char* oldStr;
    const char* newStr;
    const int bufferSize;
};

EXPORT int __stdcall Replace2(TArgs* arg)
{
    return Replace(GetCurrentProcess(), arg->oldStr, arg->newStr, arg->bufferSize);
}