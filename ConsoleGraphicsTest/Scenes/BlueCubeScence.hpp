#pragma once
#include "IScence.hpp"
#include "ConsoleEngine.hpp"


class BlueCubeScence : public IScence
{
private:

    ConsoleEngine& _consoleEngine;


public:

    BlueCubeScence(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine)
    {
    };


public:

    virtual void DrawScence(float deltaTime) override
    {
        _consoleEngine.DrawConsoleText(0,0, L"Blue cube scence", ConsoleEngine::ConsoleColour::BLUE);
    };

};