#include "App.h"

namespace HardestGame
{
    // Constructor: initialize raylib, setup default game objects, and set frame rate.
    App::App(int windowWidth, int windowHeight, const char* windowTitle)
        : _windowWidth(windowWidth)
        , _windowHeight(windowHeight)
        , _windowTitle(windowTitle)
        // Rectangle is {x, y, width, height}.
        , _player {80.0f, static_cast<float>(windowHeight) / 2.0f, 40.0f, 40.0f}
        , _moveSpeed(220.0f)
        , _obstacle(
              std::make_unique<Rectangle>(
                  Rectangle {430.0f, static_cast<float>(windowHeight) - 120.0f, 45.0f, 50.0f}))
        , _deathCount(std::make_unique<int>(0))
    {
        // Open the window using the config passed from main().
        InitWindow(_windowWidth, _windowHeight, _windowTitle.c_str());

        // 60 FPS is a common beginner-friendly target.
        SetTargetFPS(60);
    }

    // Destructor: required cleanup for raylib windows.
    App::~App()
    {
        CloseWindow();
    }

    void App::Run()
    {
        Rectangle& player = _player;
        Rectangle& obstacle = *_obstacle;

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
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        {
            _player.x -= _moveSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            _player.x += _moveSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        {
            _player.y -= _moveSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        {
            _player.y += _moveSpeed * deltaTime;
        }

        // Keep the player square inside the window bounds.
        if (_player.x < 0.0f)
        {
            // Clamp to the left edge.
            _player.x = 0.0f;
        }
        if (_player.x + _player.width > static_cast<float>(_windowWidth))
        {
            // Clamp to the right edge.
            _player.x = static_cast<float>(_windowWidth) - _player.width;
        }
        if (_player.y < 0.0f)
        {
            // Clamp to the top edge.
            _player.y = 0.0f;
        }
        if (_player.y + _player.height > static_cast<float>(_windowHeight))
        {
            // Clamp to the bottom edge.
            _player.y = static_cast<float>(_windowHeight) - _player.height;
        }

        if (CheckCollisionRecs(_player, *_obstacle))
        {
            ++(*_deathCount);
            _player.x = 80.0f;
            _player.y = static_cast<float>(_windowHeight) - 110.0f;
        }
    }

    void App::Draw() const
    {
        // Clear the frame before drawing new content.
        ClearBackground(RAYWHITE);

        // Draw player as a fully opaque green square.
        DrawRectangleRec(_player, GREEN);

        // HUD text for tutorial guidance.
        DrawText("Move: WASD or Arrow Keys", 20, 20, 24, DARKGRAY);
        DrawText("Lesson 2: obstacle exists", 20, 52, 24, DARKGREEN);

        // Render obstacle
        DrawRectangleRec(*_obstacle, RED);
        DrawText(TextFormat("Deaths: %d", *_deathCount), 20, 84, 24, MAROON);
    }

}
