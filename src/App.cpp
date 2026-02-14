#include "App.h"

namespace HardestGame
{
    // Constructor: initialize raylib, setup default game objects, and set frame rate.
    App::App(int windowWidth, int windowHeight, const char* windowTitle)
        : _windowWidth(windowWidth)
        , _windowHeight(windowHeight)
        , _windowTitle(windowTitle)
        // Rectangle is {x, y, width, height}.
        , _player{80.0f, static_cast<float>(windowHeight) / 2.0f, 40.0f, 40.0f}
        , _moveSpeed(220.0f)
    {
        // Open the window using the config passed from main().
        InitWindow(_windowWidth, _windowHeight, _windowTitle);

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
    }

    void App::Draw() const
    {
        // Clear the frame before drawing new content.
        ClearBackground(RAYWHITE);

        // Draw player as a fully opaque green square.
        DrawRectangleRec(_player, GREEN);

        // HUD text for tutorial guidance.
        DrawText("Move: WASD or Arrow Keys", 20, 20, 24, DARKGRAY);
        DrawText("Lesson 1: basic movement only", 20, 52, 24, DARKGREEN);
    }

}
