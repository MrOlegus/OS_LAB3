#include <windows.h>

const char* oldStr = "hello";
const char* newStr = "bye";

#define maxCountLength 10
#define maxMessageLength 10000
#define dllPath L"../../DllTest\\Debug\\DllTest.dll"
#define methodName "Replace"
#define BUFFER_SIZE 1024

char text[100] = "hello, how are you?\n\0";
char text2[100] = "hello, I'm OK!\n\0";
char text3[100] = "It's great, bye!\n\0";
char text4[6] = { 'h', 'e', 'l', 'l', 'o', '\n' };
char text5[100] = "itishelloween\n";
char text6[100] = "hallo? Here it's definitely not there!\n";

char startValues[maxMessageLength] = {};
char endValues[maxMessageLength] = {};

typedef int(__stdcall* TReplace)(HANDLE, char*, char*, int);

int main()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD oldProtect;
    VirtualProtect((LPVOID)oldStr, strlen(oldStr) + 1, PAGE_READONLY, &oldProtect);
    VirtualProtect((LPVOID)newStr, strlen(newStr) + 1, PAGE_READONLY, &oldProtect);

    DWORD pid = GetCurrentProcessId();
    char pidStr[20] = {0};
    _itoa_s(pid, pidStr, 10);

    while (true)
    {
        WriteConsoleA(hStdout, "\nMy PID: ", 9, NULL, NULL);
        WriteConsoleA(hStdout, pidStr, strlen(pidStr), NULL, NULL);
        WriteConsoleA(hStdout, "\nHack me!\n\n", 12, NULL, NULL);
        WriteConsoleA(hStdout, text, strlen(text), NULL, NULL);
        WriteConsoleA(hStdout, text2, strlen(text2), NULL, NULL);
        WriteConsoleA(hStdout, text3, strlen(text3), NULL, NULL);
        WriteConsoleA(hStdout, text4, strlen(text4), NULL, NULL);
        WriteConsoleA(hStdout, text5, strlen(text5), NULL, NULL);
        WriteConsoleA(hStdout, text6, strlen(text6), NULL, NULL);
        WriteConsoleA(hStdout, "\n\n\n", 4, NULL, NULL);

        Sleep(500);
    }
}