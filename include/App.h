#pragma once

#include "raylib.h"
#include <memory>
#include <string>

namespace WyrmCpp
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

        void Reset();
        void Goal();
        void NewGoalPosition();

        int BoundedRand(int min, int max) const;

        // Window configuration we keep around for drawing and placement.
        int _windowWidth;
        int _windowHeight;
        const std::string _windowTitle;

        // Player square state.
        Rectangle _player;

        // Goal square state.
        Rectangle _target;

        // Basic movement setting used by Update().
        float _moveSpeed;

        // Direction that the wyrm is heading in
        Vector2 _moveDirection;

        // Count of deaths
        std::unique_ptr<int> _deathCount;
        // Count of points
        std::unique_ptr<int> _pointCount;
    };

}
