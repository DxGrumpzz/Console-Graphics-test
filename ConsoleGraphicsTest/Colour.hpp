#pragma once
#include <stdint.h>

class Colour
{
public:

    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;

public:

    bool operator == (const Colour& colour)
    {
        if ((Red == colour.Red) &&
            (Green == colour.Green) &&
            (Blue == colour.Blue))
            return true;
        else
            return false;
    };
};