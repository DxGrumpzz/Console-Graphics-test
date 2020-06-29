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
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::RED,   1, 3.0f, rotationSpeed , PI / 2, PI / 2);
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::GREEN, 1, 3.0f, rotationSpeed , 7 * PI / 6, 7 * PI / 6);
        objects.emplace_back(consoleEngine, ConsoleEngine::ConsoleColour::BLUE,  1, 3.0f, rotationSpeed , 11 * PI / 6, 11 * PI / 6);
    };

public:
    
    virtual void UpdateScene(float deltaTime) override
    {
        DrawSpinningCubesWithLines(deltaTime);
    };

    virtual void DrawScene() override
    {
        //DrawSpinningCubesWithLines(deltaTime);
    };


private:

    void DrawSpinningCubesWithLines(float deltaTime)
    {
        Mouse mouse = _consoleEngine.GetMouse();

        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);


        for (Object& object : objects)
        {
            object.Position = mouseVector;

            object.Update(deltaTime);

            if (mouse.X != 0 && mouse.Y != 0)
                _consoleEngine.DrawLine(_transformer.CartesianVectorToScreenSpace(object.Position),
                                        _transformer.CartesianVectorToScreenSpace(mouseVector),
                                        object.ObjectColour);

            object.DrawObject();

        };

    };


    void DrawSpinningCubes(float deltaTime)
    {
        Mouse mouse = _consoleEngine.GetMouse();

        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);



        for (Object& object : objects)
        {
            object.Position = mouseVector;

            object.Update(deltaTime);

            object.DrawObject();
        };


        DrawDebugString(deltaTime);
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