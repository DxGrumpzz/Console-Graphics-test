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

    Vector2D v0 = { 0, 0 };
    Vector2D v1 = { 0, 0 };


    virtual void UpdateScene(float deltaTime) override
    {
        Mouse mouse = _consoleEngine.GetMouse();

        VectorTransformer transformer(_consoleEngine);

        Vector2D mouseCart = transformer.MouseToCartesian(mouse.X, mouse.Y);

        v1 = mouseCart - v0;

        v1.Normalize();

        v1.Scale(6.0f);
    };

    virtual void DrawScene() override
    {
        VectorTransformer transformer(_consoleEngine);

        _consoleEngine.DrawLine(transformer.CartesianVectorToScreenSpace(v0), transformer.CartesianVectorToScreenSpace(v1));
    };

};