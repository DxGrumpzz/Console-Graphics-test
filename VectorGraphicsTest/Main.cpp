#include <Windows.h>
#include <iostream>



constexpr wchar_t PIXEL_CHARACTER = L'█';


enum class ConsoleColour : WORD
{
    BLACK = 0,
    DARKBLUE = FOREGROUND_BLUE,
    DARKGREEN = FOREGROUND_GREEN,
    DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED = FOREGROUND_RED,
    DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
    DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY = FOREGROUND_INTENSITY,
    BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};


void SetConsoleFont(HANDLE consoleOutputHandle)
{
    CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof(consoleFontInfo);

    consoleFontInfo.nFont = 0;

    consoleFontInfo.dwFontSize = { 4, 5 };

    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;

    wcscpy_s(consoleFontInfo.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(consoleOutputHandle, TRUE, &consoleFontInfo);
};


void SetConsoleWindowSize(HANDLE consoleOutputHandle, SMALL_RECT consoleWindowRect)
{
    SetConsoleWindowInfo(consoleOutputHandle, TRUE, &consoleWindowRect);
};


void CenterConsoleWindow(HWND consoleHWND, int consoleWidth, int consoleHeight)
{
    HMONITOR monitorHandle = MonitorFromWindow(consoleHWND, MONITOR_DEFAULTTONEAREST);

    MONITORINFO monitorInfo = { 0 };
    monitorInfo.cbSize = sizeof(monitorInfo);

    GetMonitorInfoW(monitorHandle, &monitorInfo);

    const long monitorWidth = monitorInfo.rcWork.right;
    const long monitorHeight = monitorInfo.rcWork.bottom;

    SetWindowPos(consoleHWND, NULL, (monitorWidth / 2) - (consoleWidth * 2), (monitorHeight / 2) - (consoleHeight * 2), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
};


void HideConsoleCursor(HANDLE consoleOutputHandle)
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;

    SetConsoleCursorInfo(consoleOutputHandle, &info);
};


void SetConsolePixel(int x, int y, int width, ConsoleColour pixelColour, CHAR_INFO* screenBuffer)
{
    CHAR_INFO& pixel = screenBuffer[x * width + y];

    pixel.Char.UnicodeChar = PIXEL_CHARACTER;
    pixel.Attributes = static_cast<WORD>(pixelColour);
};


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    HWND consoleHWND = GetConsoleWindow();


    const int consoleWindowWidth = 100;
    const int consoleWindowHeight = 100;

    SMALL_RECT consoleWindowRect = { 0, 0, 1, 1 };


    SetConsoleWindowInfo(consoleOutputHandle, TRUE, &consoleWindowRect);

    COORD coord = { consoleWindowWidth, consoleWindowHeight };
    SetConsoleScreenBufferSize(consoleOutputHandle, coord);


    SetConsoleActiveScreenBuffer(consoleOutputHandle);


    SetConsoleFont(consoleOutputHandle);

    consoleWindowRect = { 0, 0, consoleWindowWidth - 1, consoleWindowHeight - 1 };

    SetConsoleWindowInfo(consoleOutputHandle, TRUE, &consoleWindowRect);

    CenterConsoleWindow(consoleHWND, consoleWindowWidth, consoleWindowHeight);

    HideConsoleCursor(consoleOutputHandle);


    CHAR_INFO* screenBuffer = new CHAR_INFO[consoleWindowWidth * consoleWindowHeight] { 0 };

    DWORD writtenAttributesCount = 0;
    DWORD writtenCharactersCount = 0;


    float playerX = consoleWindowWidth / 2;
    float playerY = consoleWindowHeight / 2;

    float playerSpeed = 0.05;


    while (1)
    {
        memset(screenBuffer, 0, sizeof(CHAR_INFO) * consoleWindowWidth * consoleWindowHeight);


        if (GetAsyncKeyState(VK_UP))
        {
            if ((playerY - playerSpeed) > 0.f)
                playerY -= playerSpeed;
        };

        if (GetAsyncKeyState(VK_DOWN))
        {
            if ((playerY + playerSpeed) < consoleWindowHeight)
                playerY += playerSpeed;
        };

        if (GetAsyncKeyState(VK_LEFT))
        {
            if ((playerX - playerSpeed) > 0.f)
                playerX -= playerSpeed;
        };

        if (GetAsyncKeyState(VK_RIGHT))
        {
            if ((playerX + playerSpeed) < consoleWindowWidth)
                playerX += playerSpeed;
        };

        SetConsolePixel(playerX, playerY, consoleWindowWidth, ConsoleColour::RED, screenBuffer);


        WriteConsoleOutputW(consoleOutputHandle, screenBuffer, { (short)consoleWindowWidth, (short)consoleWindowHeight }, { 0,0 }, &consoleWindowRect);
    };


    delete[] screenBuffer;
    screenBuffer = nullptr;
};