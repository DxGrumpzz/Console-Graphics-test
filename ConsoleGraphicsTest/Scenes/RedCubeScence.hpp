#pragma once

#include "IScence.hpp"
#include "ConsoleEngine.hpp"


class RedCubeScence : public IScence
{

private:
    ConsoleEngine& _consoleEngine;


    float playerX;
    float playerY;

    float PlayerWidth = 5;
    float PlayerHeight = 5;

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

    bool displayMousePositionOnPlayer = false;

public:

    RedCubeScence(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),

        playerX(static_cast<float>(consoleEngine.ConsoleWindowWidth / 2)),
        playerY(static_cast<float>(consoleEngine.ConsoleWindowHeight / 2))
    {
    };

    bool draggingPlayer = false;

public:

    int mouseXInPlayer = 0;
    int mouseYInPlayer = 0;

    void HandleInput(float deltaTime, ConsoleEngine& consoleEngine)
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



        if (mouse.LeftMouseButton == MouseKeyState::Pressed && mouseOverPlayer == true)
        {
            if (draggingPlayer == false)
            {
                mouseXInPlayer = mouse.X - playerX;
                mouseYInPlayer = mouse.Y - playerY;

                draggingPlayer = true;
            }
            else
            {
                draggingPlayer = true;

                playerX = mouse.X - mouseXInPlayer;
                playerY = mouse.Y - mouseYInPlayer;
            };
        }
        else
        {
            draggingPlayer = false;
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


    void HandleVelocity(float deltaTime)
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


        for (int width = 0; width < PlayerWidth; width++)
        {
            for (int height = 0; height < PlayerHeight; height++)
            {
                consoleEngine.SetConsolePixel(static_cast<int>(playerX) + width, static_cast<int>(playerY) + height, playerColour);
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


    void DrawFPS(ConsoleEngine& consoleEngine, std::wstring& debugString)
    {
        debugString.append(L"\n");

        debugString.append(L"FPS: ");
        debugString.append(std::to_wstring(consoleEngine.GetFPS()));

        debugString.append(L"\n");
    };


    #pragma endregion



public:

    virtual void DrawScence(float deltaTime) override
    {
        HandleInput(deltaTime, _consoleEngine);

        HandleCollisions(deltaTime, _consoleEngine);

        HandleVelocity(deltaTime);

        _consoleEngine.DrawConsoleText(0, 0, L"Red cube scence", ConsoleEngine::ConsoleColour::RED);

        std::wstring debugString;

        DrawPlayer(_consoleEngine);


        if (showHelp == true)
        {
            DrawConsoleText(debugString);

            DrawMousePosition(_consoleEngine, debugString);

            const Mouse& mouse = _consoleEngine.GetMouse();

            debugString.append(L"Left Mbtn: ");

            if (mouse.LeftMouseButton == MouseKeyState::Released)
                debugString.append(L"Released");
            else if (mouse.LeftMouseButton == MouseKeyState::Pressed)
                debugString.append(L"Pressed");
            else
                debugString.append(L"How tf ?");
            debugString.append(L"\n");


            DrawFPS(_consoleEngine, debugString);


            if (isFirstTimeShowHelp == true)
                debugString.append(L"Press F1 to toggle debug\n");
        };


        if (mouseOverPlayer == true)
        {
            const Mouse& mouse = _consoleEngine.GetMouse();

            _consoleEngine.SetConsolePixel(mouse.X, mouse.Y, ConsoleEngine::ConsoleColour::YELLOW);
        };


        _consoleEngine.DrawConsoleText(0, 1, debugString.c_str());
    };

};