
#include "VectorTransformer.hpp"
#include "ConsoleEngine.hpp"
#include "IScence.hpp"


class SineScene : public IScence
{

private:
    ConsoleEngine& _consoleEngine;

    VectorTransformer _transformer;


public:

    constexpr static long double PI = 22.0 / 7.0;


public:

    float sinAccumulator = 0.0f;
    float cosAccumulator = 0.0f;

    float sinResult = 0.0f;
    float cosResult = 0.0f;


    Vector2D objectPosition = { 0, 0 };

    float rotationSpeed = 5.0f;

    float rotationSize = 10.0f;


public:

    SineScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),
        _transformer(consoleEngine)
    {
    };

public:



    virtual void DrawScene(float deltaTime) override
    {
        Mouse mouse = _consoleEngine.GetMouse();

        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);

        sinResult = sin(sinAccumulator);
        cosResult = -cos(cosAccumulator);

        sinAccumulator += rotationSpeed * deltaTime;
        cosAccumulator += rotationSpeed * deltaTime;


        objectPosition.X = rotationSize * cosResult;
        objectPosition.Y = rotationSize * sinResult;
        objectPosition += mouseVector;
        
        DrawObject();

        WrapAngle(sinAccumulator);
        WrapAngle(cosAccumulator);


        DrawDebugString(deltaTime);
    };


private:


    void DrawObject()
    {
        int objectRadius = 2;

        // Draw a circle 
        for (int x = 0; x <= (objectRadius * objectRadius); x++)
        {
            for (int y = 0; y <= (objectRadius * objectRadius); y++)
            {
                // Use the pythagorean theorem to calculate a distance point between x,y and the outer radius
                int distance = pow(x - objectRadius, 2) + pow((y - objectRadius), 2);

                // Check if the distance is less that the radius inside the circle
                if (distance < objectRadius * objectRadius)
                {
                    // Convert the Cartesian position to screen space
                    // Vector2D position = { objectPosition.X, objectPosition.Y };
                    Vector2D position = _transformer.CartesianToScreenSpace(objectPosition.X, objectPosition.Y);

                    float xPosition = (x + position.X) - objectRadius;
                    float yPosition = (y + position.Y) - objectRadius;

                    // Check screen boundries
                    if ((xPosition >= 0 && xPosition < _consoleEngine.ConsoleWindowWidth) &&
                        (yPosition >= 0 && yPosition < _consoleEngine.ConsoleWindowHeight))
                    {
                        // Draw the pixel
                        _consoleEngine.SetConsolePixel(xPosition, yPosition);
                    };
                };
            };
        };
    };


    void DrawDebugString(float deltaTime)
    {
        std::wstring debugString;

        debugString.append(L"Delta time: ");
        debugString.append(std::to_wstring(deltaTime));

        debugString.append(L"\n");


        debugString.append(L"Sin accum: ");
        debugString.append(std::to_wstring(sinAccumulator));

        debugString.append(L"\n");

        debugString.append(L"Cos accum: ");
        debugString.append(std::to_wstring(cosAccumulator));

        debugString.append(L"\n");

        debugString.append(L"Sin: ");
        debugString.append(std::to_wstring(sinResult));

        debugString.append(L"\n");

        debugString.append(L"Cos: ");
        debugString.append(std::to_wstring(cosResult));


        _consoleEngine.DrawConsoleText(0, 0, debugString.c_str());
    };


    void WrapAngle(float& angle)
    {
        if (angle > (PI * 2))
            angle -= angle;
    };


    float DegressToRadians(float degrees)
    {
        return degrees * PI / 180.0f;
    };


};