#include <math.h>
#include <vector>

#include "ConsoleEngine.hpp"

#include "RedCubeScence.hpp"
#include "BlueCubeScence.hpp"
#include "GreenCubeScence.hpp"

#include "IScence.hpp"



struct Vector2
{
    float X = 0;
    float Y = 0;
};


int consoleWindowWidh = 50;
int consoleWindowHeight = 50;


std::vector<IScence*> scenes;
std::vector<IScence*>::iterator currentScene;

bool leftBracketKeyPressed = false;
bool rightBracketKeyPressed = false;



void CycleScences()
{
    if (GetAsyncKeyState(VK_CONTROL))
    {
        if (GetAsyncKeyState(VK_RIGHT))
        {
            if (rightBracketKeyPressed == false)
            {
                rightBracketKeyPressed = true;

                currentScene++;
                if (currentScene == scenes.end())
                {
                    currentScene = scenes.begin();
                };
            };
        }
        else
            rightBracketKeyPressed = false;


        if (GetAsyncKeyState(VK_LEFT))
        {
            if (leftBracketKeyPressed == false)
            {
                leftBracketKeyPressed = true;


                if (currentScene == scenes.begin())
                {
                    currentScene = scenes.end() - 1;
                }
                else
                    currentScene--;
            };
        }
        else
            leftBracketKeyPressed = false;
    };
};


bool GameLoop(float deltaTime, ConsoleEngine& consoleEngine)
{
    
    CycleScences();

    (*currentScene)->DrawScence(deltaTime);

    return true;
};



int main()
{
    ConsoleEngine* consoleEngine = new ConsoleEngine(GameLoop, consoleWindowWidh, consoleWindowHeight);

    scenes.emplace_back(new RedCubeScence(*consoleEngine));
    scenes.emplace_back(new GreenCubeScence(*consoleEngine));
    scenes.emplace_back(new BlueCubeScence(*consoleEngine));

    currentScene = scenes.begin();

    consoleEngine->BuildEngine();

    consoleEngine->Run();



    delete consoleEngine;
    consoleEngine = nullptr;
};