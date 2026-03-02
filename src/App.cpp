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
        , _player {static_cast<float>(windowWidth) / 2.0f + 2.5f, 5.0f, 20.0f, 20.0f}
        , _moveSpeed(25.0f)
        , _actionTimeLength(1.0 / 50.0)
        , _moveDirection(Vector2 {0, 0})
        , _target {5.0f, 5.0f, 20.0f, 20.0f}
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

        NewGoalPosition();

        // Main loop continues until the user closes the window.
        while (!WindowShouldClose())
        {

            const float deltaTime = GetFrameTime();

            // Update game state first.
            Update(deltaTime);

            // Start drawing this frame.
            BeginDrawing();
            // Draw the current state.
            Draw();
            // Present the frame to the screen.
            EndDrawing();

            WaitTime(0.05);
        }
    }

    void App::Update(float deltaTime)
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

        _player.x += _moveDirection.x * _moveSpeed;
        _player.y += _moveDirection.y * _moveSpeed;

        // Reset if player leaves bounds
        if (_player.x < 5.0f || _player.x + _player.width > static_cast<float>(_windowWidth) - 5.0f
            || _player.y < 5.0f
            || _player.y + _player.height > static_cast<float>(_windowHeight) - 5.0f)
        {
            Reset();
        }

        if (CheckCollisionRecs(_player, _target))
        {
            Goal();
        }
    }

    void App::Draw() const
    {
        // Clear the frame before drawing new content.
        ClearBackground(BLACK);

        // Draw player as a fully opaque green square.
        DrawRectangleRec(_player, GREEN);
        DrawRectangleRec(_target, YELLOW);

        // Render obstacle
        // DrawCircle(*_obstacle, YELLOW);
        DrawText(TextFormat("Points: %d", *_pointCount), 20, 20, 24, GREEN);
        DrawText(TextFormat("Deaths: %d", *_deathCount), 20, 52, 24, MAROON);
    }

    void App::Reset()
    {
        _moveDirection = Vector2 {0, 0};
        ++(*_deathCount);
        _player.x = static_cast<float>(_windowWidth) / 2.0f + 2.5f;
        _player.y = 5.0f;
        *_pointCount = 0;
        NewGoalPosition();
    }

    void App::Goal()
    {
        ++(*_pointCount);
        NewGoalPosition();
    }

    void App::NewGoalPosition()
    {
        //*_pointCount = BoundedRand(0, 200);
        int x = BoundedRand(1, 196) * 5 - 2.5f; // from x=5 to x=1000, 5 points between
        int y = BoundedRand(1, 196) * 5 - 2.5f; // from y=5 to y=1000, 5 points between
        _target.x = x;
        _target.y = y;
    }

    int App::BoundedRand(int min, int max) const
    {
        if (max <= min)
            return -1;
        return min + rand() % (max + 1);
    }

}
