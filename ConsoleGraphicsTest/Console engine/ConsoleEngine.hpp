#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>


enum class MouseKeyState
{
    Released = 0,
    Pressed = 1,
};


struct Mouse
{
    short X = 0;
    short Y = 0;

    MouseKeyState RightMouseButton;
    MouseKeyState LeftMouseButton;
};



/// <summary>
/// A simple class used to interact with the console's character buffer and treat it as a Pixel/screen buffer
/// </summary>
class ConsoleEngine
{

    /// <summary>
    /// A typedef for a callback function that will be ran every frame
    /// </summary>
    typedef bool (*GameLoopCallback)(float deltaTime, ConsoleEngine& consoleEngine);


public:

    /// <summary>
    /// A "Pixel" character
    /// </summary>
    static constexpr wchar_t PIXEL_CHARACTER = L'█';


private:

    /// <summary>
    /// A handle to the console output buffer
    /// </summary>
    HANDLE _consoleOutputHandle;

    /// <summary>
    /// A handle to the console input buffer
    /// </summary>
    HANDLE _consoleInputHandle;

    /// <summary>
    /// A window handle to the console's Host window
    /// </summary>
    HWND _consoleHWND;

    /// <summary>
    /// A screen rectangle of the console's window
    /// </summary>
    SMALL_RECT _consoleWindowRect;


    Mouse _mouse;

    /// <summary>
    /// How many frames elapsed in a second
    /// </summary>
    float _fps = 0.0f;

    short _fontWidth;
    short _fontHeight;

public:

    /// <summary>
    /// An enum for the consoles colours.
    /// <remarks>
    /// Stolen from: https://stackoverflow.com/a/17125539
    /// </remarks>
    /// </summary>
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

    /// <summary>
    /// The console window width
    /// </summary>
    const int ConsoleWindowWidth;

    /// <summary>
    /// The console window height
    /// </summary>
    const int ConsoleWindowHeight;

    /// <summary>
    /// The character buffer that will be displayed as a framebuffer
    /// </summary>
    CHAR_INFO* ScreenBuffer;

    /// <summary>
    /// The game-loop function callback
    /// </summary>
    GameLoopCallback GameLoop;


public:

    ConsoleEngine(GameLoopCallback gameLoop,

                  int consoleWindowWidth = 150,
                  int consoleWindowHeight = 100,

                  short fontWidth = 10,
                  short fontHeight = 10) :
        _consoleOutputHandle(NULL),
        _consoleInputHandle(NULL),

        _consoleHWND(NULL),
        _consoleWindowRect({ 0 }),

        _mouse({ 0 }),

        _fontWidth(fontWidth),
        _fontHeight(fontHeight),

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


    /// <summary>
    /// Constructs the console game engine
    /// </summary>
    void BuildEngine()
    {
        /// Note: Funnily enough order here of SetConole**** functions is very important.
        //        Weird or unexpected behaviour can occur if one or more of these functions and out of order

        // Sets the console code-page to unicode (utf8_
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        // Get the console's output handle
        _consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        _consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);

        // Get the console's window handle
        _consoleHWND = GetConsoleWindow();

        // Resize the console to the smallest possible size so the buffer stretches it and fits *perfectly
        _consoleWindowRect = { 0, 0, 1, 1 };
        SetConsoleWindowInfo(_consoleOutputHandle, TRUE, &_consoleWindowRect);

        // Set the console's buffer size
        COORD coord = { static_cast<short>(ConsoleWindowWidth), static_cast<short>(ConsoleWindowHeight) };
        SetConsoleScreenBufferSize(_consoleOutputHandle, coord);

        // Set a "uniform" console font for the pixels and characters
        SetConsoleFont(_consoleOutputHandle);

        // Resize the console widnow to the correct dimensions
        _consoleWindowRect = { 0, 0, static_cast<short>(ConsoleWindowWidth - 1), static_cast<short>(ConsoleWindowHeight - 1) };
        SetConsoleWindowInfo(_consoleOutputHandle, TRUE, &_consoleWindowRect);

        COORD actualConsoleWindowSize = GetLargestConsoleWindowSize(_consoleOutputHandle);

        CONSOLE_SCREEN_BUFFER_INFO s = { 0 };
        GetConsoleScreenBufferInfo(_consoleOutputHandle, &s);


        SetWindowLongW(_consoleHWND, GWL_STYLE, GetWindowLong(_consoleHWND, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);


        DWORD previousMode;
        /*GetConsoleMode(_consoleInputHandle, &previousMode);
        SetConsoleMode(_consoleInputHandle, ENABLE_EXTENDED_FLAGS |
                       (previousMode & ~ENABLE_QUICK_EDIT_MODE));*/

        SetConsoleMode(_consoleInputHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);


        // Center the console on the monitor
        CenterConsoleWindow(_consoleHWND, actualConsoleWindowSize.X + ConsoleWindowWidth, actualConsoleWindowSize.Y + ConsoleWindowHeight);

        // Hide the console's cursor (Blinking underscore thing )
        HideConsoleCursor(_consoleOutputHandle);
    };


    /// <summary>
    /// Run the engine and start drawing
    /// </summary>
    void Run()
    {
        // Check if a GameLoop function is defined
        if (GameLoop == nullptr)
        {
            throw std::exception("No GameLoop call back function is defined");
            return;
        };

        // Time points, neccesarry to keep consistent movment when using velocity and such
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point end;

        std::chrono::duration<float > elapsed;

        // An accumulator for the number of seconds passed
        float secondsAccumulator = 0.0f;

        // An accumulator for the number of frames that have passed
        unsigned int framesAccumulator = 0;


        while (1)
        {
            elapsed = end - start;

            start = std::chrono::steady_clock::now();

            // Clear the "frame" buffer
            memset(ScreenBuffer, 0, sizeof(CHAR_INFO) * ConsoleWindowWidth * ConsoleWindowHeight);

            // Read any input events that occured
            ReadConsoleEvents();

            // Accumulate seconds
            secondsAccumulator += elapsed.count();

            // Check if a second has passed
            if (secondsAccumulator >= 1.0f)
            {
                // Calculate FPS by deviding the number of frames passed by the elapsed time
                _fps = framesAccumulator / secondsAccumulator;

                // Reset accumulators
                secondsAccumulator = 0;
                framesAccumulator = 0;
            };


            // Run the game loop function
            if (GameLoop(elapsed.count(), *this) == false)
                return;

            framesAccumulator++;

            // Display the "frame"
            WriteConsoleOutputW(_consoleOutputHandle, ScreenBuffer, { (short)ConsoleWindowWidth, (short)ConsoleWindowHeight }, { 0,0 }, &_consoleWindowRect);


            end = std::chrono::steady_clock::now();
        };
    };


    /// <summary>
    /// Write text somewhere on screen
    /// </summary>
    /// <param name="x"> The text's x position </param>
    /// <param name="y"> The text's y position </param>
    /// <param name="text"> The text to draw </param>
    /// <param name="textColor"> Which colour to draw the text </param>
    void DrawConsoleText(int x, int y, const wchar_t* text, ConsoleColour textColor = ConsoleColour::WHITE)
    {
        // The length of the text
        size_t textLength = wcslen(text);

        // The start of the X position
        int startingX = x;

        // An index used to "lookup" characters in the text array
        int textIndex = 0;

        // Iterate through the text
        for (size_t a = 0; a < textLength; a++)
        {
            // If the current character is a newline
            if (text[textIndex] == L'\n')
            {
                // Move down a line
                y++;

                // Go to the next character so we don't actually dispaly the newline
                textIndex++;

                // Set X back to the starting position of the X
                x = startingX;

                // Move to next iteration
                continue;
            };

            // Set a character in the x and y position 
            SetCharacter(x++, y, text[textIndex], textColor);
            textIndex++;
        };
    };


    /// <summary>
    /// "Activates" a single pixel on the consoles character buffer
    /// </summary>
    /// <param name="x"> The pixel's x position </param>
    /// <param name="y"> The pixel's y position </param>
    /// <param name="pixelColour"> Which colour to draw the pixel in </param>
    void SetConsolePixel(int x, int y, ConsoleColour pixelColour = ConsoleColour::WHITE)
    {
        // Get a CHAR_INFO reference to the pointer located at the x and y position
        CHAR_INFO& pixel = *GetPixelAddress(x, y);

        // Set character and colour
        pixel.Char.UnicodeChar = PIXEL_CHARACTER;
        pixel.Attributes = static_cast<WORD>(pixelColour);
    };


    /// <summary>
    /// Draw a single character somewhere with a colour
    /// </summary>
    /// <param name="x"> The character's x position </param>
    /// <param name="y"> The character's y position </param>
    /// <param name="character"> The character to draw </param>
    /// <param name="characterColour"> Which colour to draw the character in </param>
    void SetCharacter(int x, int y, wchar_t character, ConsoleColour characterColour = ConsoleColour::WHITE)
    {
        // Get a CHAR_INFO reference to the pointer located at the x and y position
        CHAR_INFO& pixel = *GetPixelAddress(x, y);

        // Set the character and colour
        pixel.Char.UnicodeChar = character;
        pixel.Attributes = static_cast<WORD>(characterColour);
    };


public:

    const Mouse& GetMouse()
    {
        return _mouse;
    };

    unsigned int GetFPS()
    {
        return _fps;
    };

    std::pair<short, short> GetMousePosition()
    {
        return std::make_pair(_mouse.X, _mouse.Y);
    };


private:

    void ReadConsoleEvents()
    {

        INPUT_RECORD inputEvents[32];

        DWORD numberOfEvents = 0;

        GetNumberOfConsoleInputEvents(_consoleInputHandle, &numberOfEvents);

        if (numberOfEvents > 0)
            ReadConsoleInputW(_consoleInputHandle, inputEvents, numberOfEvents, &numberOfEvents);

        for (DWORD i = 0; i < numberOfEvents; i++)
        {
            if (inputEvents[i].EventType == MOUSE_EVENT)
            {
                MOUSE_EVENT_RECORD& mouseEvent = inputEvents[i].Event.MouseEvent;

                _mouse.X = mouseEvent.dwMousePosition.X;
                _mouse.Y = mouseEvent.dwMousePosition.Y;

                if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    _mouse.LeftMouseButton = MouseKeyState::Pressed;
                else
                    _mouse.LeftMouseButton = MouseKeyState::Released;


                if (mouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
                    _mouse.RightMouseButton = MouseKeyState::Pressed;
                else
                    _mouse.RightMouseButton = MouseKeyState::Released;

            };
        };

    };

    /// <summary>
    /// Sets a font for the console
    /// </summary>
    /// <param name="_consoleOutputHandle"></param>
    void SetConsoleFont(HANDLE _consoleOutputHandle)
    {
        CONSOLE_FONT_INFOEX consoleFontInfo;
        consoleFontInfo.cbSize = sizeof(consoleFontInfo);

        // Selects the first font in the console's font table
        consoleFontInfo.nFont = 0;

        // Set a font size 
        consoleFontInfo.dwFontSize = { _fontWidth, _fontHeight };

        // Use default font family and font weight
        consoleFontInfo.FontFamily = FF_DONTCARE;
        consoleFontInfo.FontWeight = FW_NORMAL;

        // Use the consolas font 
        // Without this font selection doesn't actually work
        wcscpy_s(consoleFontInfo.FaceName, L"Consolas");

        // Set the console font
        SetCurrentConsoleFontEx(_consoleOutputHandle, TRUE, &consoleFontInfo);
    };


    /// <summary>
    /// Centers the console's window
    /// </summary>
    /// <param name="consoleHWND"> The console's window handle </param>
    /// <param name="consoleWidth"> The console's window width </param>
    /// <param name="consoleHeight" The console's window height ></param>
    void CenterConsoleWindow(HWND consoleHWND, int consoleWidth, int consoleHeight)
    {
        // Get a handle to this monitor
        HMONITOR monitorHandle = MonitorFromWindow(consoleHWND, MONITOR_DEFAULTTONEAREST);

        // Get monitor info
        MONITORINFO monitorInfo = { 0 };
        monitorInfo.cbSize = sizeof(monitorInfo);

        GetMonitorInfoW(monitorHandle, &monitorInfo);

        // using variables here because its more comfortable
        const long monitorWidth = monitorInfo.rcWork.right;
        const long monitorHeight = monitorInfo.rcWork.bottom;

        // Center the window on screen
        SetWindowPos(consoleHWND, NULL, (monitorWidth / 2) - consoleWidth, (monitorHeight / 2) - consoleHeight, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    };


    /// <summary>
    /// Hides the console's cursor 
    /// </summary>
    /// <param name="consoleOutputHandle"> the console output handle </param>
    void HideConsoleCursor(HANDLE consoleOutputHandle)
    {
        // Create an invisible cursor 
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 1;
        info.bVisible = FALSE;

        // Update the console's cursor
        SetConsoleCursorInfo(_consoleOutputHandle, &info);
    };


    /// <summary>
    /// Retrives an address in the screen buffer using (x + width * y) formula
    /// </summary>
    /// <param name="x"> The pixel's x coordinate </param>
    /// <param name="y"> The pixel's y coordinate </param>
    /// <returns></returns>
    CHAR_INFO* GetPixelAddress(int x, int y)
    {
        return &ScreenBuffer[x + ConsoleWindowWidth * y];
    };

};