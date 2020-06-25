#pragma once
#include "ConsoleEngine.hpp"
#include "IScence.hpp"
#include "Vector2D.hpp"


class ObjectFollowMouseScene : public IScence
{

private:
    ConsoleEngine& _consoleEngine;

public:

    ObjectFollowMouseScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine)
    {

    };

public:

    Vector2D objectPosition = { 0.0f, 0.0f };

    Vector2D objectVelocity = { 6.0, 6.0f };



    void KeyboardVelocity(float deltaTime)
    {
        if (GetAsyncKeyState(VK_LEFT))
        {
            objectPosition.X -= objectVelocity.X * deltaTime;
        };

        if (GetAsyncKeyState(VK_RIGHT))
        {
            objectPosition.X += objectVelocity.X * deltaTime;
        };

        if (GetAsyncKeyState(VK_UP))
        {
            objectPosition.Y += objectVelocity.Y * deltaTime;
        };

        if (GetAsyncKeyState(VK_DOWN))
        {
            objectPosition.Y -= objectVelocity.Y * deltaTime;
        };

    };


    void MouseVelocity(const Vector2D& v, float deltaTime)
    {
        objectPosition += (v * deltaTime);
    };


    void DrawLine(float deltaTime)
    {
        if (GetAsyncKeyState(VK_RETURN))
        {
            DebugBreak();
        };

        VectorTransformer transformer(_consoleEngine);

        Mouse mouse = _consoleEngine.GetMouse();

        Vector2D mouseVector = transformer.MouseToCartesian(mouse);

        if (mouse.X == objectPosition.X &&
            mouse.Y == objectPosition.Y)
            return;

        Vector2D v1 = mouseVector - objectPosition;
        v1.Normalize();
        v1.Scale(4);

        if (mouse.LeftMouseButton != MouseKeyState::Held)
            return;


        objectPosition += (v1 * 2) * deltaTime;


        _consoleEngine.DrawLine(transformer.CartesianVectorToScreenSpace(objectPosition),
                                transformer.CartesianVectorToScreenSpace(v1 + objectPosition), ConsoleEngine::ConsoleColour::RED);
    };


    void DrawObject()
    {
        int radius = 8;
        VectorTransformer transformer(_consoleEngine);


        // Draw a circle 
        for (int x = 0; x <= (radius * radius); x++)
        {
            for (int y = 0; y <= (radius * radius); y++)
            {
                // Use the pythagorean theorem to calculate a distance point between x,y and the outer radius
                int distance = pow(x - radius, 2) + pow((y - radius), 2);

                // Check if the distance is less that the radius inside the circle
                if (distance < radius * radius)
                {
                    // Convert the Cartesian position to screen space
                    // Vector2D position = { objectPosition.X, objectPosition.Y };
                    Vector2D position = transformer.CartesianToScreenSpace(objectPosition.X, objectPosition.Y);

                    // Draw the pixel
                    _consoleEngine.SetConsolePixel((x + position.X) - radius, (y + position.Y) - radius);
                };

            };
        };

    };


    void DrawCartesianGrid()
    {
        for (int x = 0; x < _consoleEngine.ConsoleWindowWidth; x++)
        {
            _consoleEngine.SetConsolePixel(x, _consoleEngine.ConsoleWindowHeight / 2);
        };

        for (int y = 0; y < _consoleEngine.ConsoleWindowHeight; y++)
        {
            _consoleEngine.SetConsolePixel(_consoleEngine.ConsoleWindowWidth / 2, y);
        };

    };


    virtual void DrawScence(float deltaTime) override
    {
        //DrawCartesianGrid();

        KeyboardVelocity(deltaTime);

        DrawObject();

        DrawLine(deltaTime);


        Mouse mouse = _consoleEngine.GetMouse();

        _consoleEngine.SetConsolePixel(mouse.X, mouse.Y, ConsoleEngine::ConsoleColour::MAGENTA);



    };


};