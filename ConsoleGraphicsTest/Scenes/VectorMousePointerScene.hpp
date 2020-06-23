#pragma once

#include "IScence.hpp"
#include "ConsoleEngine.hpp"
#include "Vector2D.hpp"
#include "VectorTransformer.hpp"


class VectorMousePointerScene : public IScence
{

private:

    ConsoleEngine& _consoleEngine;



public:

    VectorMousePointerScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine)
    {
    };


    /// <summary>
    /// Activates a pixel, and converts NDC to screen space
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    void SetPixelNDC(float ndcX, float ndcY, ConsoleEngine::ConsoleColour pixelColour = ConsoleEngine::ConsoleColour::WHITE)
    {
        if (ndcX < -1 || ndcX > 1)
            throw std::exception("Out of bounds X");

        if (ndcY < -1 || ndcY > 1)
            throw std::exception("Out of bounds Y");

        // Convert normalized coordinates (NDC) to screen space coordicates
        double screenX = (static_cast<size_t>(ndcX) - (-1)) / 2.0 * (static_cast<size_t>(_consoleEngine.ConsoleWindowWidth) - 1);
        double screenY = (static_cast<long long>(-ndcY) - (-1)) / 2.0 * (static_cast<size_t>(_consoleEngine.ConsoleWindowHeight) - 1);

        _consoleEngine.SetConsolePixel(static_cast<int>(screenX), static_cast<int>(screenY), pixelColour);
    };


    /// <summary>
    /// Draws a line segment between 2 points
    /// </summary>
    /// <param name="x1"> The beginning x coordinate </param>
    /// <param name="y1"> The beginning y coordinate </param>
    /// <param name="x2"> The ending x coordinate </param>
    /// <param name="y2"> The ending y coordinate </param>
    /// <param name="lineColour"> The colour of th line </param>
    void DrawLine(float x1, float y1, float x2, float y2, ConsoleEngine::ConsoleColour lineColour = ConsoleEngine::ConsoleColour::WHITE)
    {
        // Comments on this algorithm will be lacking.
        // Because I am too shite at mathematics to understand what's going on here


        // Calculate the rise and run
        // (Slope and Y-intercept)
        float rise = y2 - y1;
        float run = x2 - x1;


        // Check if the slope of the line is too low
        if (true)//abs(rise) > abs(run))
        {
            // Swap Y-coordinates without this, every part of the line under Y1 won't render
            if (y1 > y2)
            {
                int tempX = x1;
                int tempY = y1;

                x1 = x2;
                x2 = tempX;

                y1 = y2;
                y2 = tempY;
            };

            // The line drawing equation is y = mx * b
            // But in this case we turn it upside-down so we're able to render

            float w = (x2 - x1) / (y2 - y1);
            float p = x1 - w * y1;

            for (int y = y1; y <= y2; y++)
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

            for (int x = x1; x <= x2; x++)
            {
                float y = m * x + b;


                _consoleEngine.SetConsolePixel(x, y);

            };
        };
    };

    void DrawLine(const Vector2D& point1, const Vector2D& point2, ConsoleEngine::ConsoleColour lineColour = ConsoleEngine::ConsoleColour::WHITE)
    {
        DrawLine(point1.X, point1.Y, point2.X, point2.Y, lineColour);
    };


    void DrawLineCartesian(const Vector2D& point1, const Vector2D& point2, ConsoleEngine::ConsoleColour lineColour = ConsoleEngine::ConsoleColour::WHITE)
    {
        using namespace VectorTransformer;

        Vector2D screenSpaceVector1 = CartesianToScreenSpace(point1, _consoleEngine.ConsoleWindowWidth, _consoleEngine.ConsoleWindowHeight);
        Vector2D screenSpaceVector2 = CartesianToScreenSpace(point2, _consoleEngine.ConsoleWindowWidth, _consoleEngine.ConsoleWindowHeight);

        DrawLine(screenSpaceVector1, screenSpaceVector2, lineColour);
    };


    int previousKey = 0;

    virtual void DrawScence(float deltaTime) override
    {
        
        Mouse mouse = _consoleEngine.GetMouse();

        using namespace VectorTransformer;


        Vector2D mouseCart = MouseToCartesian(mouse.X, mouse.Y, _consoleEngine.ConsoleWindowWidth, _consoleEngine.ConsoleWindowHeight);

        Vector2D v0(0, 0);
        Vector2D m(mouseCart.X, mouseCart.Y);

        Vector2D v1 = m - v0;

        v1.Normalize();

        v1.Scale(6.0f);

        DrawLineCartesian(v0, v1);
    };

};