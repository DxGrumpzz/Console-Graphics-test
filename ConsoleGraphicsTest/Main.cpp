#include "ConsoleEngine.hpp"

int consoleWindowWidh = 50;
int consoleWindowHeight = 50;


float playerX = static_cast<float>(consoleWindowWidh / 2);
float playerY = static_cast<float>(consoleWindowHeight / 2);


const float accelerationRate = 0.00001f;
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


    if ((playerX + velocityX) > consoleEngine.ConsoleWindowWidth - 1)
    {
        playerX = static_cast<float>(consoleEngine.ConsoleWindowWidth - 1);
        velocityX = 0.0f;
    };

    if ((playerX - velocityX) < 0)
    {
        playerX = 0.9f;
        velocityX = 0.0f;
    };


    if ((playerY + velocityY) > consoleEngine.ConsoleWindowHeight - 1)
    {
        playerY = static_cast<float>(consoleEngine.ConsoleWindowHeight - 1);
        velocityY = 0.0f;
    };

    if ((playerY - velocityY) < 0)
    {
        playerY = 0.9f;
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


    consoleEngine.SetConsolePixel(static_cast<int>(playerX), static_cast<int>(playerY), ConsoleEngine::ConsoleColour::RED);

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