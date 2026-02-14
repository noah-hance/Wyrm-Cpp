#pragma once

#include "raylib.h"

namespace HardestGame
{
    // App groups game setup, update, and draw code in one class.
    // This keeps main.cpp very small and easy to understand.
    class App
    {
    public:
        // Constructor receives window settings so learners can see dependency flow.
        App(int windowWidth, int windowHeight, const char* windowTitle);

        // Destructor shuts down raylib cleanly.
        ~App();

        // Runs the main game loop until the user closes the window.
        void Run();

    private:
        // Called once each frame to update game state.
        void Update(float deltaTime);

        // Called once each frame to draw the current game state.
        void Draw() const;

        // Window configuration we keep around for drawing and placement.
        int _windowWidth;
        int _windowHeight;
        const char* _windowTitle;

        // Player square state.
        Rectangle _player;

        // Basic movement setting used by Update().
        float _moveSpeed;
    };

}
