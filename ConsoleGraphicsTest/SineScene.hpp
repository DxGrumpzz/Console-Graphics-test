#include <vector>

#include "VectorTransformer.hpp"
#include "ConsoleEngine.hpp"
#include "IScene.hpp"
#include "Math.hpp"
#include "Object.hpp"


/// <summary>
/// A sine test scene used to see how sine-cosine works
/// </summary>
class SineScene : public IScene
{

private:
    ConsoleEngine& _consoleEngine;

    VectorTransformer _transformer;


public:

    /// <summary>
    /// A list of objects that will be displayed
    /// </summary>
    std::vector<Object> objects;


public:

    SineScene(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine),
        _transformer(consoleEngine)
    {
        using namespace Math;

        // Populate the objects list
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


    /// <summary>
    /// Draws the objects with lines attached
    /// </summary>
    void DrawSpinningCubesWithLines()
    {
        // Get mouse position and convert it to a Cartesian vector
        Mouse mouse = _consoleEngine.GetMouse();
        Vector2D mouseVector = _transformer.MouseToCartesian(mouse);

        // Draw the object and lines from the object's position to the mouse 
        for (Object& object : objects)
        {
            if (mouse.X != 0 && mouse.Y != 0)
                _consoleEngine.DrawLine(_transformer.CartesianVectorToScreenSpace(object.Position),
                                        _transformer.CartesianVectorToScreenSpace(mouseVector),
                                        object.ObjectColour);

            object.DrawObject();
        };
    };


    /// <summary>
    /// Draw the objects spinning around the mouse
    /// </summary>
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