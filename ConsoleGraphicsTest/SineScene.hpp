#include <vector>

#include "VectorTransformer.hpp"
#include "ConsoleEngine.hpp"
#include "IScence.hpp"
#include "Math.hpp"
#include "Object.hpp"


class SineScene : public IScence
{

private:
    ConsoleEngine& _consoleEngine;

    VectorTransformer _transformer;


public:

    std::vector<Object> objects;


public:

    SineScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),
        _transformer(consoleEngine)
    {
        using namespace Math;

        float rotationSpeed = 3.0f;
        float rotationRadiusSize = 12.0f;
        float objectRadius = 1.0f;

        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::RED,   objectRadius, rotationRadiusSize, rotationSpeed, PI / 2, PI / 2);
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::GREEN, objectRadius, rotationRadiusSize, rotationSpeed, 7 * PI / 6, 7 * PI / 6);
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::BLUE,  objectRadius, rotationRadiusSize, rotationSpeed, 11 * PI / 6, 11 * PI / 6);
    };

public:

    virtual void UpdateScene(float deltaTime) override
    {
        Mouse mouse = _consoleEngine.GetMouse();

        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);


        for (Object& object : objects)
        {
            object.Position = mouseVector;

            object.Update(deltaTime);
        };
    };

    virtual void DrawScene() override
    {
        DrawSpinningCubesWithLines();
        // DrawSpinningCubes();
    };


private:

    void DrawSpinningCubesWithLines()
    {
        Mouse mouse = _consoleEngine.GetMouse();
        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);

        for (Object& object : objects)
        {
            if (mouse.X != 0 && mouse.Y != 0)
                _consoleEngine.DrawLine(_transformer.CartesianVectorToScreenSpace(object.Position),
                                        _transformer.CartesianVectorToScreenSpace(mouseVector),
                                        object.ObjectColour);

            object.DrawObject();
        };
    };


    void DrawSpinningCubes()
    {
        for (Object& object : objects)
        {
            object.DrawObject();
        };
    };


    void DrawDebugString(float deltaTime)
    {
        std::wstring debugString;

        debugString.append(L"Delta time: ");
        debugString.append(std::to_wstring(deltaTime));

        debugString.append(L"\n");

        _consoleEngine.DrawConsoleText(0, 0, debugString.c_str());
    };

};