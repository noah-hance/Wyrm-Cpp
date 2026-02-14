# Lesson 3: Replace Raw Pointers with Smart Pointers

Lesson goal:
- refactor Lesson 2 manual ownership to smart pointers
- remove `delete` calls and make ownership explicit
- understand why smart pointers are safer defaults

## 1. Why switch

Lesson 2 showed that raw `new`/`delete` is easy to get wrong.
Smart pointers:
- tie ownership to object lifetime (RAII)
- clean up automatically
- reduce leak and double-free risk

## 2. Choose pointer types

- use `std::unique_ptr<T>` for single ownership
- use `std::shared_ptr<T>` only when ownership must be shared

For this app, `unique_ptr` is enough.

## 3. Update the header

`include/App.h`:

```cpp
#include <memory>

std::unique_ptr<Rectangle> _obstacle;
std::unique_ptr<int> _deathCount;
```

## 4. Update construction and destruction

`src/App.cpp`:

```cpp
App::App(...)
    : ...
    , _obstacle(std::make_unique<Rectangle>(
          Rectangle{430.0f, static_cast<float>(windowHeight) - 120.0f, 45.0f, 50.0f}))
    , _deathCount(std::make_unique<int>(0))
{
    InitWindow(_windowWidth, _windowHeight, _windowTitle);
    SetTargetFPS(60);
}

App::~App()
{
    CloseWindow();
}
```

No manual `delete` needed.

## 5. Keep usage almost identical

```cpp
if (CheckCollisionRecs(_player, *_obstacle))
{
    ++(*_deathCount);
}

DrawRectangleRec(*_obstacle, RED);
DrawText(TextFormat("Deaths: %d", *_deathCount), 20, 52, 24, MAROON);
```

## 6. When references still help

Even with smart pointers, references are useful for clarity:

```cpp
Rectangle& obstacle = *_obstacle;
```

This says "obstacle must exist here" and avoids repeated dereferencing.

## 7. Lesson outcome

You now have the same level behavior as Lesson 2, but with safer ownership:
- fewer manual cleanup paths
- reduced leak risk
- code that scales better as the game grows
