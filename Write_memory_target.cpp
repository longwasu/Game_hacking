#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
using namespace std;

HANDLE hProc = NULL;
DWORD pID;

bool attachProc(char* procName)
{
    PROCESSENTRY32 procEntry32;

    procEntry32.dwSize = sizeof(PROCESSENTRY32);

    auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(hProcSnap == INVALID_HANDLE_VALUE)
    {
        cout << "Failed to take snapshot of proceses" << endl;
        return false;
    }

    while(Process32Next(hProcSnap, &procEntry32))
    {
        cout << procEntry32.szExeFile << endl;

        if (!strcmp(procName, procEntry32.szExeFile))
        {
            cout << "Found process" << procEntry32.szExeFile << "with process ID" << procEntry32.th32ProcessID << endl;
            hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
            pID = procEntry32.th32ProcessID;

            if (hProc == NULL)
            {
                cout << "Failed getting handle to process." << endl;
            }

            CloseHandle(hProcSnap);
            return true;
        }
    }

    cout << "Could not find" << procName << "in the process snapshot" << endl;
    CloseHandle(hProcSnap);
    return false;
}

template <class dataType>
void wpm(dataType valToWrite, DWORD addressToWrite)
{
    WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template <class dataType>
dataType rpm(DWORD addressToRead)
{
    dataType rpmBuffer;

    ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);

    return rpmBuffer;
}

int main()
{
    DWORD memoryAddress = 0x0061FF0C;

    attachProc((char*)"Target.exe");

    while(true)
    {
        wpm<int>(100, memoryAddress);
    }

}
