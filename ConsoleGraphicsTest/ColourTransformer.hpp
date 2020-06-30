#pragma once

#include "ConsoleEngine.hpp"
#include "Colour.hpp"


namespace ColourTransformer
{

    /// <summary>
    /// Converts an rgb colour to a Console colour
    /// </summary>
    /// <param name="colour"> The rgb colour to convert </param>
    /// <returns></returns>
    ConsoleEngine::ConsoleColour RGBToConsoleColour(const Colour& colour)
    {
        if (colour.Red ==   255   &&
            colour.Green == 255 &&
            colour.Blue ==  255)
        {
            return ConsoleEngine::ConsoleColour::WHITE;
        }
        else if (colour.Red ==   0 &&
                 colour.Green == 0 &&
                 colour.Blue ==  0)
        {
            return ConsoleEngine::ConsoleColour::BLACK;
        }
        else if (colour.Red ==   0  &&
                 colour.Green == 0  &&
                 colour.Blue ==  128)
        {
            return ConsoleEngine::ConsoleColour::DARK_BLUE;
        }
        else if (colour.Red ==   0   &&
                 colour.Green == 128 &&
                 colour.Blue ==  0)
        {
            return ConsoleEngine::ConsoleColour::DARK_GREEN;
        }
        else if (colour.Red ==   0   &&
                 colour.Green == 128 &&
                 colour.Blue ==  128)
        {
            return ConsoleEngine::ConsoleColour::DARK_CYAN;
        }
        else if (colour.Red ==   128 &&
                 colour.Green == 0   &&
                 colour.Blue ==  0)
        {
            return ConsoleEngine::ConsoleColour::DARK_RED;
        }
        else if (colour.Red ==   128 &&
                 colour.Green == 0   &&
                 colour.Blue ==  128)
        {
            return ConsoleEngine::ConsoleColour::DARK_MAGENTA;
        }
        else if (colour.Red ==   128 &&
                 colour.Green == 128 &&
                 colour.Blue ==  0)
        {
            return ConsoleEngine::ConsoleColour::DARK_YELLOW;
        }
        else if (colour.Red ==   192 &&
                 colour.Green == 192 &&
                 colour.Blue ==  192)
        {
            return ConsoleEngine::ConsoleColour::DARK_GRAY;
        }
        else if (colour.Red ==   128 &&
                 colour.Green == 128 &&
                 colour.Blue ==  128)
        {
            return ConsoleEngine::ConsoleColour::GRAY;
        }
        else if (colour.Red ==   0  &&
                 colour.Green == 0  &&
                 colour.Blue ==  255)
        {
            return ConsoleEngine::ConsoleColour::BLUE;
        }
        else if (colour.Red ==   0   &&
                 colour.Green == 255 &&
                 colour.Blue ==  0)
        {
            return ConsoleEngine::ConsoleColour::GREEN;
        }
        else if (colour.Red ==   0   &&
                 colour.Green == 255 &&
                 colour.Blue ==  255)
        {
            return ConsoleEngine::ConsoleColour::CYAN;
        }
        else if (colour.Red ==   255 &&
                 colour.Green == 0   &&
                 colour.Blue ==  0)
        {
            return ConsoleEngine::ConsoleColour::RED;
        }
        else if (colour.Red ==   255 &&
                 colour.Green == 0   &&
                 colour.Blue ==  255)
        {
            return ConsoleEngine::ConsoleColour::MAGENTA;
        }
        else if (colour.Red ==   255 &&
                 colour.Green == 255 &&
                 colour.Blue ==  0)
        {
            return ConsoleEngine::ConsoleColour::YELLOW;
        }
        else
        {
            return ConsoleEngine::ConsoleColour::MAGENTA;
        };
    };



};