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
    if (((playerX + PlayerWidth) + (velocityX * deltaTime)) > consoleEngine.ConsoleWindowWidth)
    {
        playerX = static_cast<float>(consoleEngine.ConsoleWindowWidth) - PlayerWidth;
        velocityX = 0.0f;
    };

    if ((playerX - (velocityX * deltaTime)) < 0)
    {
        playerX = 0.9f;
        velocityX = 0.0f;
    };


    if (((playerY + PlayerHeight) + (velocityY * deltaTime)) > consoleEngine.ConsoleWindowHeight)
    {
        playerY = static_cast<float>(consoleEngine.ConsoleWindowHeight) - PlayerHeight;
        velocityY = 0.0f;
    };

    if ((playerY - (velocityY * deltaTime)) < 0)
    {
        playerY = 0.9f;
        velocityY = 0.0f;
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


void DrawConsoleText(ConsoleEngine& consoleEngine)
{
    std::wstring text;
    text.append(L"Player X:   ");
    text.append(std::to_wstring(playerX));

    text.append(L"\n");

    text.append(L"Player Y:   ");
    text.append(std::to_wstring(playerY));

    consoleEngine.DrawConsoleText(0, 0, text.c_str(), ConsoleEngine::ConsoleColour::RED);
    text.clear();

    text.append(L"Velocity X: ");
    text.append(std::to_wstring(velocityX));

    text.append(L"\n");

    text.append(L"Velocity Y: ");
    text.append(std::to_wstring(velocityY));

    consoleEngine.DrawConsoleText(0, 2, text.c_str(), ConsoleEngine::ConsoleColour::BLUE);
};


void DrawDeltaTime(ConsoleEngine& consoleEngine, float deltaTime)
{
    std::wstring text;
    text.append(L"Delta time: ");
    text.append(std::to_wstring(deltaTime));

    consoleEngine.DrawConsoleText(0, 4, text.c_str());
};


void DrawMousePosition(ConsoleEngine& consoleEngine)
{
    std::pair<short, short> mousePos = consoleEngine.GetMousePosition();

    std::wstring text;
    text.append(L"Mouse X: ");
    text.append(std::to_wstring(mousePos.first));

    consoleEngine.DrawConsoleText(0, 5, text.c_str());

    text.clear();

    text.append(L"Mouse Y: ");
    text.append(std::to_wstring(mousePos.second));
    consoleEngine.DrawConsoleText(0, 6, text.c_str());
}


void Draw(ConsoleEngine& consoleEngine, float deltaTime)
{
    DrawPlayer(consoleEngine);

    if (showHelp == true)
    {
        DrawConsoleText(consoleEngine);

        DrawDeltaTime(consoleEngine, deltaTime);

        DrawMousePosition(consoleEngine);

        const Mouse& mouse = consoleEngine.GetMouse();

        std::wstring text;
        text.append(L"Left Mbtn: ");

        if (mouse.LeftMouseButton == MouseKeyState::Released)
            text.append(L"Released");
        else if (mouse.LeftMouseButton == MouseKeyState::Pressed)
            text.append(L"Pressed");
        else
            text.append(L"How tf ?");

        consoleEngine.DrawConsoleText(0, 7, text.c_str());


        if (isFirstTimeShowHelp == true)
            consoleEngine.DrawConsoleText(0, 8, L"Press F1 to toggle help");
    };

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