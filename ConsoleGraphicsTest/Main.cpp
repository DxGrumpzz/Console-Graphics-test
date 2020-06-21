#include <math.h>

#include "ConsoleEngine.hpp"




struct Vector2
{
    float X = 0;
    float Y = 0;
};



int consoleWindowWidh = 50;
int consoleWindowHeight = 50;

float playerX = static_cast<float>(consoleWindowWidh / 2);
float playerY = static_cast<float>(consoleWindowHeight / 2);

float PlayerWidth = 2;
float PlayerHeight = 2;

const float accelerationRate = 20.0f;
const float decelerationRate = 2.25f;


float velocityX = 0.0;
float velocityY = 0.0;


const float maxVelocity = 20.0f;

bool mouseOverPlayer = false;

bool showHelp = true;
bool isFirstTimeShowHelp = true;

bool toggleHelpKeyPressed = false;


const float bounceForceFalloff = 7.0f;


void HandleInput(long float deltaTime, ConsoleEngine& consoleEngine)
{
    if (GetAsyncKeyState(VK_UP))
    {
        velocityY -= accelerationRate * deltaTime;
    };

    if (GetAsyncKeyState(VK_DOWN))
    {
        velocityY += accelerationRate * deltaTime;
    };

    if (GetAsyncKeyState(VK_LEFT))
    {
        velocityX -= accelerationRate * deltaTime;
    };

    if (GetAsyncKeyState(VK_RIGHT))
    {
        velocityX += accelerationRate * deltaTime;
    };


    if (GetAsyncKeyState(VK_F1))
    {
        if (toggleHelpKeyPressed == false)
        {
            toggleHelpKeyPressed = true;

            if (isFirstTimeShowHelp == true)
                isFirstTimeShowHelp = false;

            showHelp = !showHelp;
        };
    }
    else
        toggleHelpKeyPressed = false;


    const Mouse& mouse = consoleEngine.GetMouse();



    if (mouse.LeftMouseButton == MouseKeyState::Pressed &&
        mouseOverPlayer == true)
    {
        playerX = mouse.X;
        playerY = mouse.Y;
    };


    if ((mouse.X >= playerX) &&
        (mouse.X < (playerX + PlayerWidth)) &&
        (mouse.Y >= playerY) &&
        (mouse.Y < (playerY + PlayerHeight)))
    {
        mouseOverPlayer = true;
    }
    else
        mouseOverPlayer = false;

};


void HandleCollisions(float deltaTime, ConsoleEngine& consoleEngine)
{
    // Right wall
    if (((playerX + PlayerWidth) + (velocityX * deltaTime)) > consoleEngine.ConsoleWindowWidth + 1)
    {
        playerX = static_cast<float>(consoleEngine.ConsoleWindowWidth) - PlayerWidth;

        velocityX *= (-1);
        velocityX += bounceForceFalloff;
    };

    // Left wall
    if ((playerX - (velocityX * deltaTime)) < 0)
    {
        playerX = 0.9f;

        velocityX *= (-1);
        velocityX -= bounceForceFalloff;
    };

    // Bottom wall
    if (((playerY + PlayerHeight) + (velocityY * deltaTime)) > consoleEngine.ConsoleWindowHeight + 1)
    {
        playerY = static_cast<float>(consoleEngine.ConsoleWindowHeight) - PlayerHeight;

        velocityY *= (-1);
        velocityY += bounceForceFalloff;
    };

    // Top wall
    if ((playerY - (velocityY * deltaTime)) < 0)
    {
        playerY = 0.9f;

        velocityY *= (-1);
        velocityY -= bounceForceFalloff;
    };

}


void HandleVelocity(long float deltaTime)
{
    playerX += velocityX * deltaTime;
    playerY += velocityY * deltaTime;


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
        velocityX -= decelerationRate * deltaTime;

        if (velocityX < 0.0f)
            velocityX = 0.0f;
    }
    else if (velocityX < 0.0f)
    {
        velocityX += decelerationRate * deltaTime;

        if (velocityX > 0.0f)
            velocityX = 0.0f;
    };


    if (velocityY > 0.0f)
    {
        velocityY -= decelerationRate * deltaTime;

        if (velocityY < 0.0f)
            velocityY = 0.0f;
    }
    else if (velocityY < 0.0f)
    {
        velocityY += decelerationRate * deltaTime;

        if (velocityY > 0.0f)
            velocityY = 0.0f;
    };

};



#pragma region Drawing code


void DrawPlayer(ConsoleEngine& consoleEngine)
{
    ConsoleEngine::ConsoleColour playerColour;

    if (mouseOverPlayer == false)
        playerColour = ConsoleEngine::ConsoleColour::RED;
    else
        playerColour = ConsoleEngine::ConsoleColour::BLUE;


    for (size_t width = 0; width < PlayerWidth; width++)
    {
        for (size_t height = 0; height < PlayerHeight; height++)
        {
            consoleEngine.SetConsolePixel(playerX + width, playerY + height, playerColour);
        };
    };
};


void DrawConsoleText(std::wstring& debugString)
{
    debugString.append(L"Player X:   ");
    debugString.append(std::to_wstring(playerX));

    debugString.append(L"\n");

    debugString.append(L"Player Y:   ");
    debugString.append(std::to_wstring(playerY));

    debugString.append(L"\n");
    debugString.append(L"\n");

    debugString.append(L"Velocity X: ");
    debugString.append(std::to_wstring(velocityX));

    debugString.append(L"\n");

    debugString.append(L"Velocity Y: ");
    debugString.append(std::to_wstring(velocityY));

    debugString.append(L"\n");
    debugString.append(L"\n");
};


void DrawMousePosition(ConsoleEngine& consoleEngine, std::wstring& debugString)
{
    std::pair<short, short> mousePos = consoleEngine.GetMousePosition();

    debugString.append(L"Mouse X: ");
    debugString.append(std::to_wstring(mousePos.first));

    debugString.append(L"\n");

    debugString.append(L"Mouse Y: ");
    debugString.append(std::to_wstring(mousePos.second));

    debugString.append(L"\n");
    debugString.append(L"\n");
}


void DrawFPS(ConsoleEngine& consoleEngine)
{

};


void Draw(ConsoleEngine& consoleEngine, float deltaTime)
{
    std::wstring debugString;

    DrawPlayer(consoleEngine);

    if (showHelp == true)
    {
        DrawConsoleText(debugString);

        DrawMousePosition(consoleEngine, debugString);

        const Mouse& mouse = consoleEngine.GetMouse();

        debugString.append(L"Left Mbtn: ");

        if (mouse.LeftMouseButton == MouseKeyState::Released)
            debugString.append(L"Released");
        else if (mouse.LeftMouseButton == MouseKeyState::Pressed)
            debugString.append(L"Pressed");
        else
            debugString.append(L"How tf ?");
        debugString.append(L"\n");


        debugString.append(L"\n");

        debugString.append(L"FPS: ");
        debugString.append(std::to_wstring(consoleEngine.GetFPS()));

        debugString.append(L"\n");


        if (isFirstTimeShowHelp == true)
            debugString.append(L"Press F1 to toggle debug\n");
    };

    consoleEngine.DrawConsoleText(0, 0, debugString.c_str());
};


#pragma endregion



bool GameLoop(float deltaTime, ConsoleEngine& consoleEngine)
{
    HandleInput(deltaTime, consoleEngine);


    HandleCollisions(deltaTime, consoleEngine);


    HandleVelocity(deltaTime);


    Draw(consoleEngine, deltaTime);


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