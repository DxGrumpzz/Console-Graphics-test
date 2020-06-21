#pragma once
#include "IScence.hpp"


class GreenCubeScence : public IScence
{

private:

    ConsoleEngine& _consoleEngine;


public:

    GreenCubeScence(ConsoleEngine& consoleEngine) :
        _consoleEngine(consoleEngine)
    {
    };


    virtual void DrawScence(float deltaTime) override
    {
        _consoleEngine.DrawConsoleText(0,0, L"Green cube scence", ConsoleEngine::ConsoleColour::GREEN);
    };

};