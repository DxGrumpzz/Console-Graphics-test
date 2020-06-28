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


public:

    std::vector<Object> objects;

public:

    SineScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),
        _transformer(consoleEngine)
    {
        using namespace Math;

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

};