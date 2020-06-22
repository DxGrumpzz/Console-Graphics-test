#pragma once

#include <exception>
#include <math.h>


class Vector2D
{
public:

    float X = 0;
    float Y = 0;

public:

    float Length() const
    {
        return sqrtf(LengthSquare());
    };

    int LengthSquare() const
    {
        return (X * X) + (Y * Y);
    };


    void Normalize()
    {
        float length = Length();

        if (length != 0.0f)
        {
            X *= (1.0f / length);
            Y *= (1.0f / length);
        };

        // X /= length;
        // Y /= length;
    };

    Vector2D Normalized() const
    {
        int length = Length();

        if (length != 0)
        {
            Vector2D copy = { X,  Y };

            copy.X /= length;
            copy.Y /= length;

            return copy;
        }
        else
            throw std::exception("Invalid length division");
    };


    void Scale(float scalar)
    {
        X *= scalar;
        Y *= scalar;
    };


    void Rotate(float angle)
    {
        float cosResult = cos(angle);
        float sinResult = sin(angle);

        X = X * cosResult - Y * sinResult;
        Y = X * sinResult + Y * cosResult;
    };

    void RotateDeg(float degrees)
    {
        float radians = degrees * 3.14159265 / 180;

        Rotate(radians);
    };

public:

    Vector2D operator - (const Vector2D& rightVector) const
    {
        return Vector2D({ X - rightVector.X,  Y - rightVector.Y });
    };

    Vector2D& operator -= (const Vector2D& rightVector)
    {
        X -= rightVector.X;
        Y -= rightVector.Y;

        return *this;
    };

};
