#pragma once

/// <summary>
/// An interface for a game scene
/// </summary>
class IScence
{
public:

    /// <summary>
    /// Draw the current scence
    /// </summary>
    /// <param name="deltaTime"> Time passed between frames </param>
    virtual void DrawScene(float deltaTime) = 0;

};