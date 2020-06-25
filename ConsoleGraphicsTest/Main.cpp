#include <math.h>
#include <vector>

#include "ConsoleEngine.hpp"

#include "RedCubeScence.hpp"
#include "VectorMousePointerScene.hpp"
#include "ObjectFollowMouseScene.hpp"

#include "IScence.hpp"


int consoleWindowWidh = 50;
int consoleWindowHeight = 50;

short fontWidth = 10;
short fontHeight =10;

/// <summary>
/// The list of scenes
/// </summary>
std::vector<IScence*> scenes;

/// <summary>
/// The currently drawn scene
/// </summary>
std::vector<IScence*>::iterator currentScene;


bool leftBracketKeyPressed = false;
bool rightBracketKeyPressed = false;


/// <summary>
/// Cycle between available scenes
/// </summary>
void CycleScences()
{
    // If control is pressed 
    if (GetAsyncKeyState(VK_CONTROL))
    {
        // And user pressed right key 
        if (GetAsyncKeyState(VK_RIGHT))
        {
            if (rightBracketKeyPressed == false)
            {
                rightBracketKeyPressed = true;

                // Cycle to the next scene
                currentScene++;

                // If it's the last scene
                if (currentScene == scenes.end())
                {
                    // Go back to the beginning
                    currentScene = scenes.begin();
                };
            };
        }
        else
            rightBracketKeyPressed = false;


        // If user pressed the left arrow
        if (GetAsyncKeyState(VK_LEFT))
        {
            if (leftBracketKeyPressed == false)
            {
                leftBracketKeyPressed = true;

                // Check if we're at the beginning
                if (currentScene == scenes.begin())
                {
                    // Go to the end scene
                    currentScene = scenes.end() - 1;
                }
                else
                    // Cycle to previous scene
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
    ConsoleEngine* consoleEngine = new ConsoleEngine(GameLoop,
                                                     consoleWindowWidh, consoleWindowHeight,
                                                     fontWidth, fontHeight);

    consoleEngine->BuildEngine();

    scenes.emplace_back(new RedCubeScence(*consoleEngine));
    scenes.emplace_back(new VectorMousePointerScene(*consoleEngine));
    scenes.emplace_back(new ObjectFollowMouseScene(*consoleEngine));

    currentScene = scenes.end() -1;

    consoleEngine->Run();



    delete consoleEngine;
    consoleEngine = nullptr;
};