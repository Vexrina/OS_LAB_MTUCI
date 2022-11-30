#pragma warning(disable : 4996)

#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <Lmcons.h>
#include <stdio.h>

using namespace std;

wstring GetNames() {
    wchar_t buffer[256];
    unsigned long size = 256;
    wstring Names = L"Computer name is: ";
    GetComputerName(buffer, &size);
    Names = Names + buffer + L"\nUsername is: ";
    GetUserName(buffer, &size);
    Names = Names + buffer;
    return Names;
}

void WriteLine() {
    cout << "_____" << endl;
}

string GetDirectories() {
    char sysDir[MAX_PATH];
    string Dirs = "\'system32\' directory: ";
    GetSystemDirectoryA(sysDir, MAX_PATH);
    Dirs = Dirs + sysDir + "\n\'Windows\' directory: ";
    GetWindowsDirectoryA(sysDir, MAX_PATH);
    Dirs = Dirs + sysDir + "\n\'TEMP\' directory: ";
    GetTempPathA(MAX_PATH, sysDir);
    Dirs = Dirs + sysDir;
    return Dirs;
}

void MSDNVersion() {
    DWORD dwVersion = 0;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuild = 0;

    dwVersion = GetVersion();
    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.

    if (dwVersion < 0x80000000)
        dwBuild = (DWORD)(HIWORD(dwVersion));

    printf("Version is %d.%d (%d)\n",
        dwMajorVersion,
        dwMinorVersion,
        dwBuild);
}

void NormalVersion() {
    char ver[23];
    OSVERSIONINFO osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osInfo);
    switch (osInfo.dwMajorVersion) {
    case 5:
        if (osInfo.dwMinorVersion == 0)
            strcpy(ver, "Windows 2000");
        else strcpy(ver, "Windows XP");
        break;
    case 6:
        switch (osInfo.dwMinorVersion) {
        case 0:
            strcpy(ver, "Windows Vista");
            break;
        case 1:
            strcpy(ver, "Windows 7");
            break;
        case 2:
            strcpy(ver, "Windows 10");
            break;
        default:
            strcpy(ver, "Windows 8.1");
            break;
        }
    }
    cout << "OS version is: " << ver << endl;
}

void SystemMetrics() {
    int smth = GetSystemMetrics(SM_CMOUSEBUTTONS);
    string result = "Mouse buttons counter = " + to_string(smth);
    result = result + "\nCounter of monitors is: ";
    result += to_string(GetSystemMetrics(SM_CMONITORS));
    cout << result << endl;

    // Swap buttons on mouse(left to right, right to left)
    cout << SwapMouseButton(1) << endl;
    system("pause");
    // Swap buttons on mouse (left to left, right to right)
    cout << SwapMouseButton(0) << endl;
}

void MStoNormalTime(int time_ms) {
    int seconds = (time_ms / 1000) % 60;
    short minutes = time_ms / (60 * 1000) % 60;
    short hours = time_ms / (1000 * 60 * 60);
    cout << "h: " << hours << ", min: " << minutes << ", sec: " << seconds << endl;
}

void Colors() {
    int aElements[2] = { COLOR_WINDOW, COLOR_WINDOWTEXT };
    DWORD aOldColors[2];
    DWORD aNewColors[2];

    // Get the current color of the window background. 

    aOldColors[0] = GetSysColor(aElements[0]);

    printf("Current Window color: {0x%x, 0x%x, 0x%x}\n",
        GetRValue(aOldColors[0]),
        GetGValue(aOldColors[0]),
        GetBValue(aOldColors[0]));

    // Get the current color of the active caption. 

    aOldColors[1] = GetSysColor(aElements[1]);

    printf("Current active WindowText color: {0x%x, 0x%x, 0x%x}\n",
        GetRValue(aOldColors[1]),
        GetGValue(aOldColors[1]),
        GetBValue(aOldColors[1]));

    // Define new colors for the elements

    aNewColors[0] = RGB(0xFFFFFF, 0x0, 0xFFFFFF);
    aNewColors[1] = RGB(0xFFFFFF, 0xFFFFFF, 0xFFFFFFF);  // dark purple 

    printf("\nNew Window color: {0x%x, 0x%x, 0x%x}\n",
        GetRValue(aNewColors[0]),
        GetGValue(aNewColors[0]),
        GetBValue(aNewColors[0]));

    printf("New active WindowText color: {0x%x, 0x%x, 0x%x}\n",
        GetRValue(aNewColors[1]),
        GetGValue(aNewColors[1]),
        GetBValue(aNewColors[1]));

    // Set the elements defined in aElements to the colors defined
    // in aNewColors

    SetSysColors(2, aElements, aNewColors);

    printf("\nWindow background and active border have been changed.\n");
    printf("Reverting to previous colors in 10 seconds...\n");
    
    Sleep(10000);
    //system("pause");

    // Restore the elements to their original colors

    SetSysColors(2, aElements, aOldColors);
}

void AdditionalApi() {
    // 1
    string result;
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    result = result + "\nmonitor X,Y is: ";
    result += to_string(x) + "x" + to_string(y);
    cout << result << endl;
    WriteLine();

    //2
    SYSTEMTIME localtime;
    GetLocalTime(&localtime);
    cout << localtime.wHour << ":" << localtime.wMinute << ":" << localtime.wSecond << endl;

    WriteLine();

    //3
    cout << "here was Beep" << endl;
    MessageBeep(MB_OK);

    WriteLine();

    //4 where .exe file (как бы это запускалось через терминал)
    wcout << GetCommandLine() << endl;
    
    WriteLine();
}

int main()
{
    // Computer Name and User Name
    wcout << GetNames() << endl;
    WriteLine();
    // System Directories
    cout << GetDirectories() << endl;
    WriteLine();
    // OS version
    cout << "MSDN variant of GetVersion: " << endl;
    MSDNVersion();
    WriteLine();
    NormalVersion();
    WriteLine();
    // SystemMetrics
    SystemMetrics();
    WriteLine();
    // SystemParametrs
    BOOL fontsmoothing;
    BOOL clearType;//ClearType — это программная технология, улучшающая читаемость текста на ЖК-мониторах.
    SystemParametersInfoA(SPI_GETFONTSMOOTHING, 0, &fontsmoothing, 0);
    SystemParametersInfoA(SPI_SETCLEARTYPE, 0, &clearType, 0);
    cout << "True = 1, False = 0"<<endl;
    cout << "beep is " << clearType << " Fontsmoothing is " << fontsmoothing << endl;
    WriteLine();
    // SystemColors
    Colors();
    WriteLine();
    // TimeFunction
    MStoNormalTime(GetTickCount64());
    SYSTEMTIME var;
    GetSystemTime(&var);
    cout << var.wHour << ":" << var.wMinute << ":" << var.wSecond << endl;
    WriteLine();
    // Additional API
    AdditionalApi();
    system("pause");
    return 0;
}