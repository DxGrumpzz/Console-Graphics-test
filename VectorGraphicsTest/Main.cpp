#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>


class ConsoleEngine
{
    typedef bool (*GameLoopCallback)(long double deltaTime, ConsoleEngine& consoleEngine);

public:

    static constexpr wchar_t PIXEL_CHARACTER = L'█';


private:

    HANDLE _consoleOutputHandle;
    HWND _consoleHWND;

    SMALL_RECT _consoleWindowRect;

public:

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

    const int ConsoleWindowWidth;
    const int ConsoleWindowHeight;

    CHAR_INFO* ScreenBuffer;

    GameLoopCallback GameLoop;


public:

    ConsoleEngine(GameLoopCallback gameLoop,
                  int consoleWindowWidth = 150,
                  int consoleWindowHeight = 100) :
        _consoleOutputHandle(NULL),
        _consoleHWND(NULL),
        _consoleWindowRect({ 0 }),

        ConsoleWindowWidth(consoleWindowWidth),
        ConsoleWindowHeight(consoleWindowHeight),

        ScreenBuffer(new CHAR_INFO[static_cast<size_t>(consoleWindowWidth) * static_cast<size_t>(consoleWindowHeight)] { 0 }),

        GameLoop(gameLoop)
    {

    };

    ~ConsoleEngine()
    {
        delete[] ScreenBuffer;
        ScreenBuffer = nullptr;
    };


public:

    void BuildEngine()
    {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        _consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        _consoleHWND = GetConsoleWindow();


        _consoleWindowRect = { 0, 0, 1, 1 };

        SetConsoleWindowInfo(_consoleOutputHandle, TRUE, &_consoleWindowRect);

        COORD coord = { static_cast<short>(ConsoleWindowWidth), static_cast<short>(ConsoleWindowHeight) };
        SetConsoleScreenBufferSize(_consoleOutputHandle, coord);


        SetConsoleActiveScreenBuffer(_consoleOutputHandle);


        SetConsoleFont(_consoleOutputHandle);

        _consoleWindowRect = { 0, 0, static_cast<short>(ConsoleWindowWidth - 1), static_cast<short>(ConsoleWindowHeight - 1) };

        SetConsoleWindowInfo(_consoleOutputHandle, TRUE, &_consoleWindowRect);

        CenterConsoleWindow(_consoleHWND, ConsoleWindowWidth, ConsoleWindowHeight);

        HideConsoleCursor(_consoleOutputHandle);
    };


    void Run()
    {
        if (!GameLoop)
            return;

        std::chrono::system_clock::time_point timePoint1 = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point tp2;

        DWORD written = 0;
        while (1)
        {
            tp2 = std::chrono::system_clock::now();

            std::chrono::duration<long double> elapsedTime = tp2 - timePoint1;

            timePoint1 = tp2;


            memset(ScreenBuffer, 0, sizeof(CHAR_INFO) * ConsoleWindowWidth * ConsoleWindowHeight);

            GameLoop(elapsedTime.count(), *this);

            WriteConsoleOutputW(_consoleOutputHandle, ScreenBuffer, { (short)ConsoleWindowWidth, (short)ConsoleWindowHeight }, { 0,0 }, &_consoleWindowRect);
        };
    };


    void WriteText(int x, int y, const wchar_t* text, ConsoleColour textColor = ConsoleColour::WHITE)
    {
        size_t textLength = wcslen(text);

        int startingX = x;

        int textIndex = 0;
        for (size_t a = 0; a < textLength; a++)
        {
            if (text[textIndex] == L'\n')
            {
                y++;
                textIndex++;
                x = startingX;
                a++;
            };

            SetCharacter(x++, y, text[textIndex], textColor);
            textIndex++;
        };
    };


    void SetConsolePixel(int x, int y, ConsoleColour pixelColour)
    {
        CHAR_INFO& pixel = ScreenBuffer[x + ConsoleWindowWidth * y];

        pixel.Char.UnicodeChar = PIXEL_CHARACTER;
        pixel.Attributes = static_cast<WORD>(pixelColour);
    };


    void SetCharacter(int x, int y, wchar_t character, ConsoleColour characterColour = ConsoleColour::WHITE)
    {
        CHAR_INFO& pixel = ScreenBuffer[x + ConsoleWindowWidth * y];

        pixel.Char.UnicodeChar = character;
        pixel.Attributes = static_cast<WORD>(characterColour);
    };

private:

    void SetConsoleFont(HANDLE _consoleOutputHandle)
    {
        CONSOLE_FONT_INFOEX consoleFontInfo;
        consoleFontInfo.cbSize = sizeof(consoleFontInfo);

        consoleFontInfo.nFont = 0;

        consoleFontInfo.dwFontSize = { 10, 10 };

        consoleFontInfo.FontFamily = FF_DONTCARE;
        consoleFontInfo.FontWeight = FW_NORMAL;

        wcscpy_s(consoleFontInfo.FaceName, L"Consolas");

        SetCurrentConsoleFontEx(_consoleOutputHandle, TRUE, &consoleFontInfo);
    };


    void SetConsoleWindowSize(HANDLE _consoleOutputHandle, SMALL_RECT consoleWindowRect)
    {
        SetConsoleWindowInfo(_consoleOutputHandle, TRUE, &consoleWindowRect);
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


    void HideConsoleCursor(HANDLE _consoleOutputHandle)
    {
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 1;
        info.bVisible = FALSE;

        SetConsoleCursorInfo(_consoleOutputHandle, &info);
    };


    CHAR_INFO* GetPixelAddress(int x, int y)
    {
        return &ScreenBuffer[x + ConsoleWindowWidth * y];
    };

};



int consoleWindowWidh = 50;
int consoleWindowHeight = 50;


float playerX = consoleWindowWidh / 2;
float playerY = consoleWindowHeight / 2;


const float accelerationRate = 0.00001;
const float decelerationRate = 0.000001f;


float velocityX = 0.0;
float velocityY = 0.0;


const float maxVelocity = 200.0f;



bool GameLoop(long double deltaTime, ConsoleEngine& consoleEngine)
{

    if (GetAsyncKeyState(VK_UP))
    {
        velocityY -= accelerationRate;
    };

    if (GetAsyncKeyState(VK_DOWN))
    {
        velocityY += accelerationRate ;
    };

    if (GetAsyncKeyState(VK_LEFT))
    {
        velocityX -= accelerationRate ;
    };

    if (GetAsyncKeyState(VK_RIGHT))
    {
        velocityX += accelerationRate ;
    };



    if ((playerX + velocityX) > consoleEngine.ConsoleWindowWidth - 0.9)
    {
        playerX = consoleEngine.ConsoleWindowWidth - 0.9;
        velocityX = 0.0f;
    };

    if ((playerX - velocityX) < 0)
    {
        playerX = 0.9;
        velocityX = 0.0f;
    };


    if ((playerY + velocityY) > consoleEngine.ConsoleWindowHeight - 0.9)
    {
        playerY = consoleEngine.ConsoleWindowHeight - 0.9;
        velocityY = 0.0f;
    };

    if ((playerY - velocityY) < 0)
    {
        playerY = 0.9;
        velocityY = 0.0f;
    };



    playerX += velocityX;
    playerY += velocityY;




    if (velocityX > maxVelocity)
        velocityX = maxVelocity;

    if (velocityX < (-maxVelocity))
        velocityX = -maxVelocity;

    if (velocityY > maxVelocity)
        velocityY = maxVelocity;

    if (velocityY < (-maxVelocity))
        velocityY = -maxVelocity;



    if (velocityX > 0.0f)
    {
        velocityX -= decelerationRate;

        if (velocityX < 0.0f)
            velocityX = 0.0f;
    }
    else if (velocityX < 0.0f)
    {
        velocityX += decelerationRate;

        if (velocityX > 0.0f)
            velocityX = 0.0f;
    };


    if (velocityY > 0.0f)
    {
        velocityY -= decelerationRate;

        if (velocityY < 0.0f)
            velocityY = 0.0f;
    }
    else if (velocityY < 0.0f)
    {
        velocityY += decelerationRate;

        if (velocityY > 0.0f)
            velocityY = 0.0f;
    };


    consoleEngine.SetConsolePixel(playerX, playerY, ConsoleEngine::ConsoleColour::RED);

    std::wstring text;
    text.append(L"Player X: ");
    text.append(std::to_wstring(playerX));

    text.append(L"\n");

    text.append(L"Player Y: ");
    text.append(std::to_wstring(playerY));

    consoleEngine.WriteText(0, 0, text.c_str(), ConsoleEngine::ConsoleColour::RED);
    text.clear();

    text.append(L"Velocity X: ");
    text.append(std::to_wstring(velocityX));

    text.append(L"\n");

    text.append(L"Velocity Y: ");
    text.append(std::to_wstring(velocityY));

    consoleEngine.WriteText(0, 2, text.c_str(), ConsoleEngine::ConsoleColour::BLUE);

    return true;
};




int main()
{
    ConsoleEngine* consoleEngine = new ConsoleEngine(GameLoop, consoleWindowWidh, consoleWindowHeight);

    consoleEngine->BuildEngine();

    consoleEngine->Run();


    delete consoleEngine;
    consoleEngine = nullptr;
};