#include <Windows.h>
#include <iostream>
#include <chrono>


class ConsoleEngine
{
    typedef bool (*GameLoopCallback)(long double deltaTime, ConsoleEngine& consoleEngine);

public:

    const wchar_t PIXEL_CHARACTER = L'█';


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

    const int ConsoleWindowWidth = 150;
    const int ConsoleWindowHeight = 100;

    CHAR_INFO* ScreenBuffer;

    GameLoopCallback GameLoop;


public:

    ConsoleEngine(GameLoopCallback gameLoop,
                  int consoleWindowWidth = 150, int consoleWindowHeight = 100) :
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
        std::chrono::steady_clock::time_point tp1 = std::chrono::high_resolution_clock::now();
        std::chrono::steady_clock::time_point tp2;

        while (1)
        {
            tp2 = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<long double> elapsedTime = tp2 - tp1;

            tp1 = tp2;

            memset(ScreenBuffer, 0, sizeof(CHAR_INFO) * ConsoleWindowWidth * ConsoleWindowHeight);

            GameLoop(elapsedTime.count(), *this);

            WriteConsoleOutputW(_consoleOutputHandle, ScreenBuffer, { (short)ConsoleWindowWidth, (short)ConsoleWindowHeight }, { 0,0 }, &_consoleWindowRect);
        };

    };


    void SetConsolePixel(int x, int y, int width, ConsoleColour pixelColour, CHAR_INFO* screenBuffer)
    {
        CHAR_INFO& pixel = screenBuffer[x + width * y];

        pixel.Char.UnicodeChar = PIXEL_CHARACTER;
        pixel.Attributes = static_cast<WORD>(pixelColour);
    };

    void SetConsolePixel(int x, int y, ConsoleColour pixelColour)
    {
        CHAR_INFO& pixel = ScreenBuffer[x + ConsoleWindowWidth * y];

        pixel.Char.UnicodeChar = PIXEL_CHARACTER;
        pixel.Attributes = static_cast<WORD>(pixelColour);
    };


private:

    void SetConsoleFont(HANDLE _consoleOutputHandle)
    {
        CONSOLE_FONT_INFOEX consoleFontInfo;
        consoleFontInfo.cbSize = sizeof(consoleFontInfo);

        consoleFontInfo.nFont = 0;

        consoleFontInfo.dwFontSize = { 6, 6 };

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
};



int consoleWindowWidh = 150;
int consoleWindowHeight = 100;


float playerX = consoleWindowWidh / 2;
float playerY = consoleWindowHeight / 2;


const float accelerationRate = 0.09f;
const float decelerationRate = 0.001f;


float velocityX = 0.0;
float velocityY = 0.0;


const float maxVelocity = 0.15f;


bool GameLoop(long double deltaTime, ConsoleEngine& consoleEngine)
{

    if (GetAsyncKeyState(VK_UP))
    {
        velocityY -= accelerationRate;
    };

    if (GetAsyncKeyState(VK_DOWN))
    {
        velocityY += accelerationRate;
    };

    if (GetAsyncKeyState(VK_LEFT))
    {
        velocityX -= accelerationRate;
    };

    if (GetAsyncKeyState(VK_RIGHT))
    {
        velocityX += accelerationRate;
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

    return true;
};



int main()
{
    ConsoleEngine* consoleEngine = new ConsoleEngine(GameLoop);

    consoleEngine->BuildEngine();

    consoleEngine->Run();

    delete consoleEngine;
    consoleEngine = nullptr;

    /*
    DWORD writtenAttributesCount = 0;
    DWORD writtenCharactersCount = 0;


    float playerX = consoleWindowWidth / 2;
    float playerY = consoleWindowHeight / 2;

    const float accelerationRate = 0.09f;
    const float decelerationRate = 0.001f;

    bool isAccelerating = true;

    float velocityX = 0.0;
    float velocityY = 0.0;

    const float maxVelocity = 0.15f;



    while (1)
    {
        memset(screenBuffer, 0, sizeof(CHAR_INFO) * consoleWindowWidth * consoleWindowHeight);

        if (GetAsyncKeyState(VK_UP))
        {
            velocityY -= accelerationRate;
        };

        if (GetAsyncKeyState(VK_DOWN))
        {
            velocityY += accelerationRate;
        };

        if (GetAsyncKeyState(VK_LEFT))
        {
            velocityX -= accelerationRate;

        };

        if (GetAsyncKeyState(VK_RIGHT))
        {
            velocityX += accelerationRate;
        };



        if ((playerX + velocityX) > consoleWindowWidth - 0.9)
        {
            playerX = consoleWindowWidth - 0.9;
            velocityX = 0.0f;
        };

        if ((playerX - velocityX) < 0)
        {
            playerX = 0.9;
            velocityX = 0.0f;
        };


        if ((playerY + velocityY) > consoleWindowHeight - 0.9)
        {
            playerY = consoleWindowHeight - 0.9;
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


        SetConsolePixel(playerX, playerY, consoleWindowWidth, ConsoleColour::RED, screenBuffer);


        wchar_t s[256] = { 0 };
        swprintf_s(s, 256, L"Velociy X: %3.3f, Velociy Y: %3.3f, Player X: %3.3f, Player Y: %3.3f", velocityX, velocityY, playerX, playerY);

        SetConsoleTitleW(s);

        WriteConsoleOutputW(consoleOutputHandle, screenBuffer, { (short)consoleWindowWidth, (short)consoleWindowHeight }, { 0,0 }, &consoleWindowRect);
    };
    */
};