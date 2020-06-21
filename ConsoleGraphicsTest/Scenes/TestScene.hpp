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
        if (x < 0 || x > 1)
            throw std::exception("Out of bounds X");

        if (y < 0 || y > 1)
            throw std::exception("Out of bounds Y");

        // Convert normalized coordinates (NDC) to screen space coordicates
        int screenX = (x - (-1)) / 2.0 * (_consoleEngine.ConsoleWindowWidth - 1);
        int screenY = ((-y) - (-1)) / 2.0 * (_consoleEngine.ConsoleWindowHeight - 1);


        _consoleEngine.SetConsolePixel(screenX, screenY, pixelColour);
    };

    virtual void DrawScence(float deltaTime) override
    {
        SetPixel(1, 0);
    };

};