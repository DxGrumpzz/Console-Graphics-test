#include <math.h>
#include <vector>

#include "ConsoleEngine.hpp"

#include "RedCubeScence.hpp"
#include "VectorMousePointerScene.hpp"
#include "ObjectFollowMouseScene.hpp"
#include "SineScene.hpp"
#include "DisplayBitmapScene.hpp"

#include "IScene.hpp"


int consoleWindowWidh = 512;
int consoleWindowHeight = 175;

short fontWidth = 3;
short fontHeight = 3;

/// <summary>
/// The list of scenes
/// </summary>
std::vector<IScene*> scenes;

/// <summary>
/// The currently drawn scene
/// </summary>
std::vector<IScene*>::iterator currentScene;


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

    (*currentScene)->UpdateScene(deltaTime);
    (*currentScene)->DrawScene();

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
    scenes.emplace_back(new SineScene(*consoleEngine));
    scenes.emplace_back(new DisplayBitmapScene(*consoleEngine));

    currentScene = scenes.end() -1;

    consoleEngine->Run();



    delete consoleEngine;
    consoleEngine = nullptr;
};