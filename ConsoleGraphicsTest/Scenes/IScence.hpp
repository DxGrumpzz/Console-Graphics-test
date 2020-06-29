#pragma once

/// <summary>
/// An interface for a game scene
/// </summary>
class IScence
{
public:

    virtual void UpdateScene(float deltaTime) = 0;

    /// <summary>
    /// Draw the current scence
    /// </summary>
    /// <param name="deltaTime"> Time passed between frames </param>
    virtual void DrawScene() = 0;

};