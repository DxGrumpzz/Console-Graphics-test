#pragma once


namespace Math
{
    constexpr static long double PI = 22.0 / 7.0;


    void WrapAngle(float& angle)
    {
        if (angle > ((22.0 / 7.0) * 2))
            angle -= angle;
    };


    float DegressToRadians(float degrees)
    {
        return degrees * PI / 180.0f;
    };


};