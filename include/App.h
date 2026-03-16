#pragma once

#include "raylib.h"
#include <memory>
#include <string>

namespace WyrmCpp
{
    // TailSection is used to create a linkedlist of tail segments that disappear over time.
    // Could be done without, but this decreases process load significantly
    struct TailSection
    {
      public:
        int position;
        int countdownValue;
        struct TailSection* next;

        TailSection(int pos, int startValue, TailSection* n)
        {
            position = pos;
            countdownValue = startValue;
            next = n;
        }
    };

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
        void Update();

        // Called once each frame to draw the current game state.
        void Draw() const;
        void DrawWyrm() const;

        void MovePlayer(Vector2 movement);
        void UpdateTail();

        void Reset();
        void Goal();
        void ChangeGoalPosition();
        int SetBoardPlayerPosition(Vector2 position);

        void ResetBoard();
        void DeleteTail(TailSection* ts);

        int BoundedRand(int min, int max) const;

        // Window configuration we keep around for drawing and placement.
        int _windowWidth;
        int _windowHeight;
        const std::string _windowTitle;

        // Player square state.
        Rectangle _player;
        Vector2 _playerPosition;

        // Goal square state.
        Rectangle _target;
        Vector2 _targetPosition;

        // background square state.
        Rectangle _background;

        const int _boardDimension = 37;
        int _board[37 * 37];

        // Basic movement setting used by Update().
        int _moveSpeed;
        // Basic timing setting used by Update().
        double _actionTimeLength;

        // Designates the extending tail of the Wyrm
        int _tailLength;
        TailSection _tailStart;

        // Direction that the wyrm is heading in
        Vector2 _moveDirection;

        // Count of deaths
        std::unique_ptr<int> _deathCount;
        // Count of points
        std::unique_ptr<int> _pointCount;
        // Max count of points
        std::unique_ptr<int> _maxPointCount;
    };

}
