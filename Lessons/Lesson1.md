# Lesson 1: Project Setup and a Movable Square

Lesson goal:
- set up the app structure
- understand `main.cpp` vs `App`
- render one square and move it left/right

## 1. What you are building

In this lesson, you are only building a minimal playground:
- open a window
- draw a player square
- move the square with keyboard input

No obstacles or collisions yet.

## 2. File roles

- `src/main.cpp`: tiny entry point that creates `HardestGame::App` and calls `Run()`. In C++, `main` is the automatic entry point called when the executable starts. Learn more: [YouTube: C++ main function explained for beginners](https://www.youtube.com/results?search_query=c%2B%2B+main+function+explained+for+beginners).
- `include/App.h`: class declaration (public API + private members).
- `src/App.cpp`: update loop and drawing implementation.

This separation is a core C++ habit: declarations in headers and implementation in source files. Learn more: [YouTube: C++ headers and source files for beginners](https://www.youtube.com/results?search_query=c%2B%2B+header+files+and+source+files+for+beginners).

## 3. Step-by-step implementation

### Step A: Keep `main.cpp` minimal

`src/main.cpp`:

```cpp
#include "App.h"

int main()
{
    // Configure the window size once.
    constexpr int windowWidth = 960;
    constexpr int windowHeight = 540;

    // Create the app object with width, height, and window title.
    HardestGame::App app(windowWidth, windowHeight, "World's Hardest Game Tutorial");

    // Start the game loop.
    app.Run();

    // Return success to the operating system.
    return 0;
}
```

### Step B: Define a simple app API

`include/App.h`:

```cpp
#pragma once

#include "raylib.h"

namespace HardestGame
{
    class App
    {
    public:
        // Constructor accepts window setup values from main.cpp.
        App(int windowWidth, int windowHeight, const char* windowTitle);

        // Destructor closes the raylib window.
        ~App();

        // Runs update + draw every frame.
        void Run();

    private:
        // Handles input and movement logic.
        void Update(float deltaTime);

        // Draws the current frame.
        void Draw() const;

        // Window config copied from constructor arguments.
        int _windowWidth;
        int _windowHeight;
        const char* _windowTitle;

        // Player rectangle state.
        Rectangle _player;

        // Movement speed in pixels per second.
        float _moveSpeed;
    };

}
```

### Step C: Implement update and draw

`src/App.cpp`:

```cpp
#include "App.h"

namespace HardestGame
{
    App::App(int windowWidth, int windowHeight, const char* windowTitle)
        // Store constructor values into member variables.
        : _windowWidth(windowWidth)
        , _windowHeight(windowHeight)
        , _windowTitle(windowTitle)
        // Rectangle is {x, y, width, height}.
        , _player{100.0f, 250.0f, 48.0f, 48.0f}
        // Movement speed is pixels per second.
        , _moveSpeed(260.0f)
    {
        // Create the raylib window.
        InitWindow(_windowWidth, _windowHeight, _windowTitle);
        // Cap to 60 FPS so motion feels consistent.
        SetTargetFPS(60);
    }

    App::~App()
    {
        // Always close the window during cleanup.
        CloseWindow();
    }

    void App::Run()
    {
        while (!WindowShouldClose())
        {
            // Time since last frame (in seconds).
            const float deltaTime = GetFrameTime();
            // Update game state using frame time.
            Update(deltaTime);

            // Start rendering a new frame.
            BeginDrawing();
            // Draw the current game state.
            Draw();
            // Submit this frame to the screen.
            EndDrawing();
        }
    }

    void App::Update(float deltaTime)
    {
        // Move left while A or Left Arrow is held.
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        {
            _player.x -= _moveSpeed * deltaTime;
        }
        // Move right while D or Right Arrow is held.
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            _player.x += _moveSpeed * deltaTime;
        }

        // Clamp left edge.
        if (_player.x < 0.0f)
        {
            _player.x = 0.0f;
        }
        // Clamp right edge.
        if (_player.x + _player.width > static_cast<float>(_windowWidth))
        {
            _player.x = static_cast<float>(_windowWidth) - _player.width;
        }
    }

    void App::Draw() const
    {
        // Clear old pixels from the previous frame.
        ClearBackground(RAYWHITE);
        // Draw the player as a solid green square.
        DrawRectangleRec(_player, GREEN);
        // Draw small instruction text.
        DrawText("Move: A/D or Left/Right", 20, 20, 24, DARKGRAY);
    }

}
```

## 4. Build and run

Use any setup from `README.md`, then run and verify:
- a single green square appears
- it moves left/right

## 5. Checkpoint

Before moving to Lesson 2, make sure you can explain:
- why `deltaTime` is used
- why we clamp position to the window bounds
- why `main.cpp` should stay small
