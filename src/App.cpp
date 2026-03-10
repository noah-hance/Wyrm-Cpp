#include "App.h"
#include <chrono>
#include <ctime>

namespace WyrmCpp
{

    // Constructor: initialize raylib, setup default game objects, and set frame rate.
    App::App(int windowWidth, int windowHeight, const char* windowTitle)
        : _windowWidth(windowWidth)
        , _windowHeight(windowHeight)
        , _windowTitle(windowTitle)
        // Rectangle is {x, y, width, height}.
        , _player {static_cast<float>(windowWidth) / 2.0f + 1.0f, 87.5f, 20.0f, 20.0f}
        , _playerPosition(Vector2 {18, 0})
        , _moveSpeed(1)
        , _moveDirection(Vector2 {0, 0})
        , _actionTimeLength(0.05)
        , _target {5.0f, 5.0f, 20.0f, 20.0f}
        , _targetPosition(Vector2 {0, 0})
        , _background {50.0f, 85.0f, 900.0f, 900.0f}
        , _pointCount(std::make_unique<int>(0))
        , _deathCount(std::make_unique<int>(0))
    {
        // Open the window using the config passed from main().
        InitWindow(_windowWidth, _windowHeight, _windowTitle.c_str());

        // 60 FPS is a common beginner-friendly target.
        SetTargetFPS(60);
        Vector2 itemPosition = Vector2 {static_cast<float>(windowHeight) - 120.0f, 50.0f};
    }

    // Destructor: required cleanup for raylib windows.
    App::~App()
    {
        CloseWindow();
    }

    void App::Run()
    {
        Rectangle& player = _player;
        Rectangle& target = _target;

        ResetBoard();
        MovePlayer(Vector2 {0, 0});
        ChangeGoalPosition();

        // Main loop continues until the user closes the window.
        while (!WindowShouldClose())
        {

            // Update game state first.
            Update();

            // Start drawing this frame.
            BeginDrawing();
            // Draw the current state.
            Draw();
            // Present the frame to the screen.
            EndDrawing();

            WaitTime(_actionTimeLength);
        }
    }

    void App::Update()
    {
        // Movement: WASD and arrow keys are both supported.
        if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && _moveDirection.x == 0)
        {
            _moveDirection.x = -1;
            _moveDirection.y = 0;
        }
        else if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && _moveDirection.x == 0)
        {
            _moveDirection.x = 1;
            _moveDirection.y = 0;
        }
        else if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && _moveDirection.y == 0)
        {
            _moveDirection.x = 0;
            _moveDirection.y = -1;
        }
        else if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && _moveDirection.y == 0)
        {
            _moveDirection.x = 0;
            _moveDirection.y = 1;
        }
        else if (IsKeyDown(KEY_SPACE))
        {
            _moveDirection.x = 0;
            _moveDirection.y = 0;
        }

        if (_moveDirection.x == 0 && _moveDirection.y == 0)
        {
            return;
        }

        MovePlayer(_moveDirection);
    }

    void App::Draw()
    {
        // Clear the frame before drawing new content.
        ClearBackground(BLACK);

        DrawRectangleRec(_background, GetColor(0x444444ff));
        DrawWyrm();
        DrawRectangleRec(_target, YELLOW);

        DrawText(TextFormat("Points: %d", *_pointCount), 20, 20, 24, GREEN);
        // DrawText(TextFormat("Points: %f", _playerPosition.x), 20, 20, 24, GREEN);
        DrawText(TextFormat("Deaths: %d", *_deathCount), 20, 52, 24, MAROON);
        // DrawText(TextFormat("Deaths: %f", _playerPosition.y), 20, 52, 24, MAROON);
    }

    void App::DrawWyrm()
    {
        Color tailColor = Color {0, 228, 48, 155};
        for (int i = 0; i < _boardDimension * _boardDimension; i++)
        {
            if (_board[i] == -1)
            {
                Rectangle wyrmSection = Rectangle {
                    53.5f + (i % _boardDimension) * 24.305555f,
                    87.5f + (i / _boardDimension) * 24.305555f,
                    20.0f,
                    20.0f};
                // Draw player as a fully opaque green square.
                DrawRectangleRec(wyrmSection, tailColor);
            }
        }
        DrawRectangleRec(_player, GREEN);
    }

    void App::MovePlayer(Vector2 movement)
    {
        Vector2 newPlayerPosition = Vector2 {
            _playerPosition.x + _moveDirection.x * _moveSpeed,
            _playerPosition.y + _moveDirection.y * _moveSpeed};

        int moveStatus = SetBoardPlayerPosition(newPlayerPosition);
        if (moveStatus < 0) // touched death
        {
            Reset();
            return;
        }
        if (moveStatus > 0) // touched target
        {
            Goal();
        }

        _playerPosition = newPlayerPosition;

        _player.x = 53.5 + _playerPosition.x * 24.305555f;
        _player.y = 87.5 + _playerPosition.y * 24.305555f;
    }

    void App::Reset()
    {
        ResetBoard();
        _playerPosition = Vector2 {18, 0};
        _moveDirection = Vector2 {0, 0};
        MovePlayer(_moveDirection);

        ++(*_deathCount);
        *_pointCount = 0;
        ChangeGoalPosition();
    }

    void App::Goal()
    {
        ++(*_pointCount);
        ChangeGoalPosition();
    }

    void App::ChangeGoalPosition()
    {
        int prevBoardPosition = _boardDimension * _targetPosition.y + _targetPosition.x;
        _board[prevBoardPosition] = 0;

        int newGoalPositionX = BoundedRand(0, _boardDimension);
        int newGoalPositionY = BoundedRand(0, _boardDimension);

        int boardPosition = _boardDimension * newGoalPositionY + newGoalPositionX;
        _board[boardPosition] = 1;

        _target.x = 53.5f + newGoalPositionX * 24.305555f;
        _target.y = 87.5f + newGoalPositionY * 24.305555f;
    }

    int App::SetBoardPlayerPosition(Vector2 position)
    {
        if (position.x < 0 || position.x >= _boardDimension || position.y < 0
            || position.y >= _boardDimension)
        {
            return -2;
        }

        int boardPosition = _boardDimension * position.y + position.x;
        int status = _board[boardPosition];

        _board[boardPosition] = -1;
        return status;
    }

    void App::ResetBoard()
    {
        for (int i = 0; i < _boardDimension * _boardDimension; i++)
        {
            _board[i] = 0;
        }
    }

    int App::BoundedRand(int min, int max) const
    {
        if (max <= min)
            return -1;
        return min + rand() % (max);
    }

}
