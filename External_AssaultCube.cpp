#include <iostream>
#include <windows.h>
#include <string>
#include <ctime>

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);
void WriteToMemory(HANDLE hProcHandle);



std::string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;

bool IsGameAvail;
bool UpdateOnNextRun;

//AMMO VARS
bool AmmoStatus;

DWORD AmmoBaseAddress[] = {0x00636C90};
DWORD AmmoOffsets[] = {0x140};

//HEALTH VARS
bool HealthStatus;

DWORD HealthBaseAddress = {0x00636C90};
DWORD HealthOffsets[] = {0xEC};

int main()
{
    HWND hGameWindow = NULL;
    int timeSinceLastUpdate = clock();
    int GameAvailTMR = clock();
    int onePressTMR = clock();
    DWORD dwProcID = NULL;
    HANDLE hProcHandle = NULL;
    UpdateOnNextRun = true;
    std::string sAmmoStatus = "OFF";
    std::string sHealthStatus = "OFF";

    while(!GetAsyncKeyState(VK_INSERT))
    {
        if(clock() - GameAvailTMR > 100)
        {
            GameAvailTMR = clock();
            IsGameAvail = false;

            hGameWindow = FindWindow(NULL, LGameWindow);
            if (hGameWindow)
            {
                GetWindowThreadProcessId( hGameWindow, &dwProcID);
                if (dwProcID != 0)
                {
                    hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
                    if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
                    {
                        GameStatus = "Failed to open process for valid handle";
                    }
                    else
                    {
                        GameStatus = "AssaultCube Ready To Hack";
                        IsGameAvail = true;
                    }
                }
                else
                {
                    GameStatus = "Failed to get process ID";
                }
            }
            else
            {
                GameStatus = "AssaultCube NOT FOUND";
            }

            if(UpdateOnNextRun || clock() - timeSinceLastUpdate > 5000)
            {
                system("cls");
                std::cout << "-------------------------------" << std::endl;
                std::cout << "    AssaultCube memory hakcer  " << std::endl;
                std::cout << "-------------------------------\n" << std::endl << std::endl;
                std::cout << "GAME STATUS:" << GameStatus << std::endl << std::endl;
                std::cout << "[F1] Ulimited Ammo -->" << sAmmoStatus << "<-" << std::endl << std::endl;
                std::cout << "[F2] Ulimited Health -->" << sHealthStatus << "<-" << std::endl << std::endl;
                std::cout << "[INSERT] Exit" << std::endl;
                UpdateOnNextRun = false;
                timeSinceLastUpdate = clock();
            }

            if(IsGameAvail)
            {
                //WRITE TO MEMORY
                WriteToMemory(hProcHandle);
            }
        }

        if(clock() - onePressTMR > 400)
        {
            if(IsGameAvail)
            {
                //AMMO
                if(GetAsyncKeyState(VK_F1))
                {
                    onePressTMR = clock();
                    AmmoStatus = !AmmoStatus;
                    UpdateOnNextRun = true;
                    if(AmmoStatus)sAmmoStatus = "ON";
                    else sAmmoStatus = "OFF";
                }
                //HEALTH
                else if(GetAsyncKeyState(VK_F2))
                {
                    onePressTMR = clock();
                    HealthStatus = !HealthStatus;
                    UpdateOnNextRun = true;
                    if(HealthStatus)sHealthStatus = "ON";
                    else sHealthStatus = "OFF";
                }
            }
        }
    }
    CloseHandle(hProcHandle);
    CloseHandle(hGameWindow);

    return ERROR_SUCCESS;


}




DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
    DWORD pointer = BaseAddress;
    DWORD pTemp;

    DWORD pointerAddr;
    for(int c = 0; c < PointerLevel; c++)
    {
        if(c == 0)
        {
            ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
        }
        pointerAddr = pTemp + Offsets[c];
        ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
    }
    return pointerAddr;
}


void WriteToMemory(HANDLE hProcHandle)
{
    DWORD AddressToWrite;
    if(AmmoStatus)

    {
        AddressToWrite = FindDmaAddy(3, hProcHandle, AmmoOffsets, AmmoBaseAddress);
        WriteProcessMemory(hProcHandle, (BYTE)AddressToWrite, &AmmoValue, sizeof(AmmoValue), NULL);
    }
    if(HealthStatus)
    {
        AddressToWrite = FindDmaAddy(3, hProcHandle, HealthOffsets, HealthBaseAddress);
        WriteProcessMemory(hProcHandle, (BYTE)AddressToWrite, &HealthValue, sizeof(HealthValue), NULL);
    }

}



