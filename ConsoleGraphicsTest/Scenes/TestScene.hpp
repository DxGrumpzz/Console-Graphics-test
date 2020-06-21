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



    virtual void DrawScence(float deltaTime) override
    {
        SetPixel(-0.5, 0);
    };

};