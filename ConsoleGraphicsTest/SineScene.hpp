#include <vector>

#include "VectorTransformer.hpp"
#include "ConsoleEngine.hpp"
#include "IScence.hpp"


class Object
{
private:

    ConsoleEngine& _consoleEngine;
    VectorTransformer _transformer;


    float _sinAccumulator;
    float _cosAccumulator;

    float _rotationSize;
    float _rotationSpeed;


public:

    Vector2D Position = { 0, 0 };

    float Radius;

    ConsoleEngine::ConsoleColour ObjectColour;


public:

    Object(ConsoleEngine& consoleEngine,
           ConsoleEngine::ConsoleColour objectColour,

           float radius = 1.0f,

           float rotationSize = 3.0f,
           float rotationSpeed = 3.0f,

           float sinOffset = 0.0f,
           float cosOffset = 0.0f) :

        _consoleEngine(consoleEngine),
        _transformer(consoleEngine),

        Radius(radius),

        _sinAccumulator(sinOffset),
        _cosAccumulator(cosOffset),

        _rotationSize(rotationSize),
        _rotationSpeed(rotationSpeed),

        ObjectColour(objectColour)
    {
    };

public:

    void DrawObject(float deltaTime)
    {
        float sinResult = sin(_sinAccumulator);
        float cosResult = -cos(_cosAccumulator);


        Position.X += (_rotationSize * (cosResult));
        Position.Y += (_rotationSize * (sinResult));


        _sinAccumulator += deltaTime * _rotationSpeed;
        _cosAccumulator += deltaTime * _rotationSpeed;


        WrapAngle(_sinAccumulator);
        WrapAngle(_cosAccumulator);


        WrapAngle(_sinAccumulator);
        WrapAngle(_cosAccumulator);


        int radiusSquared = Radius * Radius;

        // Draw a circle
        for (int x = 0; x <= radiusSquared; x++)
        {
            for (int y = 0; y <= radiusSquared; y++)
            {
                // Use the pythagorean theorem to calculate a distance point between x,y and the outer radius
                int distance = pow(x - Radius, 2) + pow((y - Radius), 2);

                // Check if the distance is less that the radius inside the circle
                if (distance < radiusSquared)
                {
                    // Convert the Cartesian position to screen space
                    // Vector2D position = { objectPosition.X, objectPosition.Y };
                    Vector2D position = _transformer.CartesianToScreenSpace(Position.X, Position.Y);

                    float xPosition = (x + position.X) - Radius;
                    float yPosition = (y + position.Y) - Radius;

                    // Check screen boundries
                    if ((xPosition >= 0 && xPosition < _consoleEngine.ConsoleWindowWidth) &&
                        (yPosition >= 0 && yPosition < _consoleEngine.ConsoleWindowHeight))
                    {
                        // Draw the pixel
                        _consoleEngine.SetConsolePixel(xPosition, yPosition, ObjectColour);
                    };
                };
            };
        };
    };


    void WrapAngle(float& angle)
    {
        if (angle > ((22.0 / 7.0) * 2))
            angle -= angle;
    };

};


class SineScene : public IScence
{

private:
    ConsoleEngine& _consoleEngine;

    VectorTransformer _transformer;


public:

    constexpr static long double PI = 22.0 / 7.0;

public:

    std::vector<Object> objects;

public:

    SineScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),
        _transformer(consoleEngine)
    {
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::RED,   1, 3.0f, 3.0f, PI/2, PI/2);
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::GREEN, 1, 3.0f, 3.0f, 7 * PI / 6, 7 * PI / 6);
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::BLUE,  1, 3.0f, 3.0f, 11 * PI / 6, 11 * PI / 6);
    };

public:


    virtual void DrawScene(float deltaTime) override
    {
        Mouse mouse = _consoleEngine.GetMouse();

        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);



        for (Object& object : objects)
        {
            object.Position = mouseVector;

            object.DrawObject(deltaTime);
        };


        DrawDebugString(deltaTime);
    };


private:


    void DrawDebugString(float deltaTime)
    {
        std::wstring debugString;

        debugString.append(L"Delta time: ");
        debugString.append(std::to_wstring(deltaTime));

        debugString.append(L"\n");

        _consoleEngine.DrawConsoleText(0, 0, debugString.c_str());
    };


    float DegressToRadians(float degrees)
    {
        return degrees * PI / 180.0f;
    };

};