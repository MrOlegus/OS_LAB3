#include <windows.h>
#include "../DllTest/dllmain.cpp"

const char* oldStr = "hello";
const char* newStr = "bye";

#define maxCountLength 10
#define maxMessageLength 10000
#define BUFFER_SIZE 1024

char text[100] = "hello, how are you?\n\0";
char text2[100] = "hello, I'm OK!\n\0";
char text3[100] = "It's great, bye!\n\0";
char text4[6] = { 'h', 'e', 'l', 'l', 'o', '\n' };
char text5[100] = "itishelloween\n";
char text6[100] = "hallo? Here it's definitely not there!\n";

char startValues[maxMessageLength] = {};
char endValues[maxMessageLength] = {};

int main()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    WriteConsoleA(hStdout, "Before: \n", 10, NULL, NULL);
    WriteConsoleA(hStdout, text, strlen(text), NULL, NULL);
    WriteConsoleA(hStdout, text2, strlen(text2), NULL, NULL);
    WriteConsoleA(hStdout, text3, strlen(text3), NULL, NULL);
    WriteConsoleA(hStdout, text4, strlen(text4), NULL, NULL);
    WriteConsoleA(hStdout, text5, strlen(text5), NULL, NULL);
    WriteConsoleA(hStdout, text6, strlen(text6), NULL, NULL);

    DWORD oldProtect;
    VirtualProtect((LPVOID)oldStr, strlen(oldStr) + 1, PAGE_READONLY, &oldProtect);
    VirtualProtect((LPVOID)newStr, strlen(newStr) + 1, PAGE_READONLY, &oldProtect);

    //////////////////////////////////////////////////////// начальные значения
    //strcat_s(startValues, text); strcat_s(startValues, "\r\n");
    //strcat_s(startValues, text2); strcat_s(startValues, "\r\n");
    //strcat_s(startValues, text3); strcat_s(startValues, "\r\n");
    //strcat_s(startValues, text4); strcat_s(startValues, "\r\n");
    //strcat_s(startValues, text5); strcat_s(startValues, "\r\n");
    //strcat_s(startValues, text6); strcat_s(startValues, "\r\n");
    //DWORD oldProtect2;
    //bool r = VirtualProtect((LPVOID)startValues, strlen(startValues) + 1, PAGE_READONLY, &oldProtect2);
    //////////////////////////////////////////////////////// начальные значения

    HANDLE hProcess = GetCurrentProcess();
    int q = Replace(hProcess, oldStr, newStr, BUFFER_SIZE);

    char qStr[10];
    _itoa_s(q, qStr, 10);

    WriteConsoleA(hStdout, "\nCount of replacements: \0", 30, NULL, NULL);
    WriteConsoleA(hStdout, qStr, strlen(qStr), NULL, NULL);
    WriteConsoleA(hStdout, "\n", 1, NULL, NULL);

    WriteConsoleA(hStdout, "\nAfter: \n", 10, NULL, NULL);
    WriteConsoleA(hStdout, text, strlen(text), NULL, NULL);
    WriteConsoleA(hStdout, text2, strlen(text2), NULL, NULL);
    WriteConsoleA(hStdout, text3, strlen(text3), NULL, NULL);
    WriteConsoleA(hStdout, text4, strlen(text4), NULL, NULL);
    WriteConsoleA(hStdout, text5, strlen(text5), NULL, NULL);
    WriteConsoleA(hStdout, text6, strlen(text6), NULL, NULL);
    WriteConsoleA(hStdout, "\n", 1, NULL, NULL);
}