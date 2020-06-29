#pragma once
#include "ConsoleEngine.hpp"
#include "IScence.hpp"
#include "Vector2D.hpp"


/// <summary>
/// A scene that draws an object that follows  the mouse around
/// </summary>
class ObjectFollowMouseScene : public IScence
{

private:
    ConsoleEngine& _consoleEngine;

    VectorTransformer _transformer;

public:

    Vector2D objectPosition = { 0.0f, 0.0f };

    Vector2D objectVelocity = { 6.0, 6.0f };


public:

    ObjectFollowMouseScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),
        _transformer(_consoleEngine)
    {

    };



    void HandleInput(float deltaTime)
    {
        Mouse mouse = _consoleEngine.GetMouse();

        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);

        // Just so we don't divide by zero, check if mouse pointer is inside the object's (0,0)
        if (mouse.X == objectPosition.X &&
            mouse.Y == objectPosition.Y)
            return;

        // Update velocity only if left mouse button is held down
        if (mouse.LeftMouseButton != MouseKeyState::Held)
            return;

        // Update velocity based on the position of the mouse and object
        objectVelocity = mouseVector - objectPosition;

        // Normalize the velocity and scale it by 4 so we get a consistent speed
        objectVelocity.Normalize();
        objectVelocity.Scale(4);

        // Update the velocity
        objectPosition += (objectVelocity * 2) * deltaTime;

    };


    void DrawLine()
    {
        Mouse mouse = _consoleEngine.GetMouse();

        // Draw the line pointing to the mouse's cursor only if left mouse button is held down
        if (mouse.LeftMouseButton == MouseKeyState::Held)
        {
            _consoleEngine.DrawLine(_transformer.CartesianVectorToScreenSpace(objectPosition),
                                    _transformer.CartesianVectorToScreenSpace(objectVelocity + objectPosition), ConsoleEngine::ConsoleColour::RED);
        };
    };


    void DrawObject()
    {
        int radius = 5;


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
                    Vector2D position = _transformer.CartesianToScreenSpace(objectPosition.X, objectPosition.Y);

                    // Draw the pixel
                    _consoleEngine.SetConsolePixel((x + position.X) - radius, (y + position.Y) - radius);
                };

            };
        };

    };


    void DrawCartesianGrid()
    {
        // Draw 2 lines going through the middle of the screen, emulates a cartesian grid

        for (int x = 0; x < _consoleEngine.ConsoleWindowWidth; x++)
        {
            _consoleEngine.SetConsolePixel(x, _consoleEngine.ConsoleWindowHeight / 2);
        };

        for (int y = 0; y < _consoleEngine.ConsoleWindowHeight; y++)
        {
            _consoleEngine.SetConsolePixel(_consoleEngine.ConsoleWindowWidth / 2, y);
        };

    };


    virtual void UpdateScene(float deltaTime) override
    {
        // Hanlde user input
        HandleInput(deltaTime);
    };


    virtual void DrawScene() override
    {
        //DrawCartesianGrid();

        // Draw the object 
        DrawObject();

        // Draw the vector pointing to the mouse's position after drawing the object so the line is visible
        DrawLine();

        // Draw a purple square at the position mouse's pointer
        Mouse mouse = _consoleEngine.GetMouse();
        _consoleEngine.SetConsolePixel(mouse.X, mouse.Y, ConsoleEngine::ConsoleColour::MAGENTA);

    };

};