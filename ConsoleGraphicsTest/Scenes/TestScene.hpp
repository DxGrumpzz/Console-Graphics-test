#pragma once


#include "IScence.hpp"
#include "ConsoleEngine.hpp"
#include "Vector2D.hpp"




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
        if (abs(rise) > abs(run))
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


    void DrawLine(Vector2D point1, Vector2D point2, ConsoleEngine::ConsoleColour lineColour = ConsoleEngine::ConsoleColour::WHITE)
    {
        DrawLine(point1.X, point1.Y, point2.X, point2.Y, lineColour);
    };



    Vector2D ToScreenSpace(const Vector2D& vector)
    {
        float screenSpaceX = vector.X + (_consoleEngine.ConsoleWindowWidth / 2);
        float screenSpaceY = (-vector.Y) + (_consoleEngine.ConsoleWindowHeight / 2);
        
        return { screenSpaceX, screenSpaceY };
    };



    Vector2D v1 = { 0 , 0 };


    virtual void DrawScence(float deltaTime) override
    {
        Vector2D v2 = { 0, -10 };
        
        Vector2D v1SP = ToScreenSpace(v2);
        
        _consoleEngine.SetConsolePixel(v1SP.X, v1SP.Y);
    };

};