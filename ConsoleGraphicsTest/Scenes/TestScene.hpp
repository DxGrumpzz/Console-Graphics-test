#pragma once
#include "IScence.hpp"
#include "ConsoleEngine.hpp"


struct Vector2
{
    float X = 0;
    float Y = 0;
};


class TestScene : public IScence
{

private:

    ConsoleEngine& _consoleEngine;



public:

    TestScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine)
    {
    };


    /// <summary>
    /// Activates a pixel, and converts NDC to screen space
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    void SetPixel(float x, float y, ConsoleEngine::ConsoleColour pixelColour = ConsoleEngine::ConsoleColour::WHITE)
    {
        if (x < -1 || x > 1)
            throw std::exception("Out of bounds X");

        if (y < -1 || y > 1)
            throw std::exception("Out of bounds Y");

        // Convert normalized coordinates (NDC) to screen space coordicates
        double screenX = (static_cast<size_t>(x) - (-1)) / 2.0 * (static_cast<size_t>(_consoleEngine.ConsoleWindowWidth) - 1);
        double screenY = (static_cast<long long>(-y) - (-1)) / 2.0 * (static_cast<size_t>(_consoleEngine.ConsoleWindowHeight) - 1);

        _consoleEngine.SetConsolePixel(static_cast<int>(screenX), static_cast<int>(screenY), pixelColour);
    };


    void DrawLine(float x1, float y1, float x2, float y2, ConsoleEngine::ConsoleColour lineColour = ConsoleEngine::ConsoleColour::WHITE)
    {
        float rise = y2 - y1;
        float run = x2 - x1;

        if (abs(rise) > abs(run))
        {
            if (y1 > y2)
            {
                int tempX = x1;
                int tempY = y1;

                x1 = x2;
                x2 = tempX;

                y1 = y2;
                y2 = tempY;
            }


            float w = (x2 - x1) / (y2 - y1);
            float p = x1 - w * y1;

            for (int y = y1; y < y2; y++)
            {
                float x = w * y + p;
                _consoleEngine.SetConsolePixel(x, y);
            };
        }
        else
        {
            if (x1 > x2)
            {
                int tempX = x1;
                int tempY = y1;

                x1 = x2;
                x2 = tempX;

                y1 = y2;
                y2 = tempY;
            };


            float m = rise / run;

            float b = y1 - m * x1;

            for (int x = x1; x < x2; x++)
            {
                float y = m * x + b;
                _consoleEngine.SetConsolePixel(x, y);
            };
        };
    };

    float x1 = _consoleEngine.ConsoleWindowWidth;
    float y1 = _consoleEngine.ConsoleWindowHeight;

    float x2 = _consoleEngine.ConsoleWindowWidth / 2; 
    float y2 = _consoleEngine.ConsoleWindowHeight / 2;


    virtual void DrawScence(float deltaTime) override
    {
        const Mouse& mouse = _consoleEngine.GetMouse();

        if (mouse.LeftMouseButton == MouseKeyState::Pressed)
        {
            x1 = mouse.X;
            y1 = mouse.Y;
        }
        else if (mouse.RightMouseButton == MouseKeyState::Pressed)
        {
            x2 = mouse.X;
            y2 = mouse.Y;
        };


        DrawLine(x1, y1, x2, y2);
    };

};