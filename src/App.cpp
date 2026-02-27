#include "App.h"
#include <ctime>

namespace WyrmCpp
{

    // Constructor: initialize raylib, setup default game objects, and set frame rate.
    App::App(int windowWidth, int windowHeight, const char* windowTitle)
        : _windowWidth(windowWidth)
        , _windowHeight(windowHeight)
        , _windowTitle(windowTitle)
        // Rectangle is {x, y, width, height}.
        , _player {static_cast<float>(windowWidth) / 2.0f, 5.0f, 20.0f, 20.0f}
        , _moveSpeed(750.0f) // 1500.0
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

        std::srand(std::time({})); // seed from current time
        NewGoalPosition();

        // Main loop continues until the user closes the window.
        while (!WindowShouldClose())
        {
            // Seconds since the last frame (delta time).
            const float deltaTime = GetFrameTime();

            // Update game state first.
            Update(deltaTime);

            // Start drawing this frame.
            BeginDrawing();
            // Draw the current state.
            Draw();
            // Present the frame to the screen.
            EndDrawing();

            // int* leakedCounter = new int(42);
            //*leakedCounter += 1;
            // delete leakedCounter;

            // char* leakedBlock = new char[1024 * 1024]; // 1mb / frame
            // leakedBlock[0] = 1;
            // delete[] leakedBlock;
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
        if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && _moveDirection.x == 0)
        {
            _moveDirection.x = 1;
            _moveDirection.y = 0;
        }
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && _moveDirection.y == 0)
        {
            _moveDirection.x = 0;
            _moveDirection.y = -1;
        }
        if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && _moveDirection.y == 0)
        {
            _moveDirection.x = 0;
            _moveDirection.y = 1;
        }

        _player.x += _moveDirection.x * _moveSpeed * deltaTime;
        _player.y += _moveDirection.y * _moveSpeed * deltaTime;

        // Keep the player square inside the window bounds.
        /* if (_player.x < 5.0f)
        {
            // Clamp to the left edge.
            _player.x = 5.0f;
        }
        if (_player.x + _player.width > static_cast<float>(_windowWidth) - 5.0f)
        {
            // Clamp to the right edge.
            _player.x = static_cast<float>(_windowWidth) - _player.width - 5.0f;
        }
        if (_player.y < 5.0f)
        {
            // Clamp to the top edge.
            _player.y = 5.0f;
        }
        if (_player.y + _player.height > static_cast<float>(_windowHeight) - 5.0f)
        {
            // Clamp to the bottom edge.
            _player.y = static_cast<float>(_windowHeight) - _player.height - 5.0f;
        }*/

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

        // HUD text for tutorial guidance.
        /*DrawText("Move: WASD or Arrow Keys", 20, 20, 24, DARKGRAY);
        DrawText("Lesson 2: obstacle exists", 20, 52, 24, DARKGREEN);*/

        // Render obstacle
        // DrawCircle(*_obstacle, YELLOW);
        DrawText(TextFormat("Points: %d", *_pointCount), 20, 20, 24, GREEN);
        DrawText(TextFormat("Deaths: %d", *_deathCount), 20, 52, 24, MAROON);
    }

    void App::Reset()
    {
        _moveDirection = Vector2 {0, 0};
        ++(*_deathCount);
        _player.x = static_cast<float>(_windowWidth) / 2.0f;
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
        int x = BoundedRand(1, 196) * 5; // from x=5 to x=1000, 5 points between
        int y = BoundedRand(1, 196) * 5; // from y=5 to y=1000, 5 points between
        // *_pointCount = x;
        // *_deathCount = y;
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
