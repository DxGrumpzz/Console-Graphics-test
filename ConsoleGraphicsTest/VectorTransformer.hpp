#pragma once

#include "Vector2D.hpp"

namespace VectorTransformer
{

    Vector2D CartesianToScreenSpace(float x, float y, int consoleWindowWidth, int consoleWindowHeight)
    {
        float screenSpaceX = x + (consoleWindowWidth / 2);
        float screenSpaceY = (-y) + (consoleWindowHeight / 2);

        return { screenSpaceX, screenSpaceY };
    };


    Vector2D CartesianVectorToScreenSpace(const Vector2D& vector, int consoleWindowWidth, int consoleWindowHeight)
    {
        return CartesianToScreenSpace(vector.X, vector.Y, consoleWindowWidth, consoleWindowHeight);
    };

    Vector2D NDCToScreenSpace(const Vector2D& vector, int consoleWindowWidth, int consoleWindowHeight)
    {
        return { ((vector.X + 1.0f) * ((consoleWindowWidth) / 2)),  ((-vector.Y + 1.0f) * ((consoleWindowHeight) / 2)) };
    };


    Vector2D MouseToVector(short x, short y)
    {
        return { static_cast<float>(x), static_cast<float>(y) };
    };

    Vector2D MouseToCartesian(short x, short y, int consoleWindowWidth, int consoleWindowHeight)
    {
        return { static_cast<float>(x) - consoleWindowWidth / 2, -(static_cast<float>(y) - consoleWindowHeight / 2) };
    };

};