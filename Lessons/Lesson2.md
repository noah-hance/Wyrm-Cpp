# Lesson 2: Pointers, References, and Manual Memory Management

Lesson goal:
- understand references vs pointers in practical game code
- build level 1 with one obstacle
- manage dynamic memory manually with `new`/`delete`
- intentionally create a severe memory leak, observe it, then fix it

## 1. References vs pointers

Rules of thumb:
- use a reference (`Type&`) when the object must exist and should not be null
- use a pointer (`Type*`) when null is meaningful or ownership/lifetime is dynamic

Examples:

```cpp
void UpdatePlayer(Rectangle& player, float dt);   // reference: must be valid
bool HitObstacle(const Rectangle* obstacle);      // pointer: can be nullptr
```

## 2. Add manual ownership to `App`

Update `include/App.h` with raw pointers:

```cpp
Rectangle* _obstacle;
int* _deathCount;
```

Initialize and clean up manually in `src/App.cpp`:

```cpp
App::App(...)
    : ...
    , _obstacle(new Rectangle{430.0f, static_cast<float>(windowHeight) - 120.0f, 45.0f, 50.0f})
    , _deathCount(new int(0))
{
    InitWindow(_windowWidth, _windowHeight, _windowTitle);
    SetTargetFPS(60);
}

App::~App()
{
    delete _obstacle;
    delete _deathCount;
    CloseWindow();
}
```

Use references for required objects:

```cpp
Rectangle& player = _player;
Rectangle& obstacle = *_obstacle;
```

## 3. Build level 1 with one obstacle that kills the player

In `Update`, add collision:

```cpp
if (CheckCollisionRecs(_player, *_obstacle))
{
    ++(*_deathCount);
    _player.x = 80.0f;
    _player.y = static_cast<float>(_windowHeight) - 110.0f;
}
```

In `Draw`, render:

```cpp
DrawRectangleRec(*_obstacle, RED);
DrawText(TextFormat("Deaths: %d", *_deathCount), 20, 52, 24, MAROON);
```

## 4. Intentionally create a memory leak (bad step)

Add this temporary code inside `Update`:
WARNING: This is so we can observe first hand how bad memory leaks are and why its so important to remember we need to cleanup after ourselves! Also be sure to save work and such before doing this, could lead to OS crash (unlikely but possible).

```cpp
int* leakedCounter = new int(42);
*leakedCounter += 1;
```

Do not delete it yet. This leaks memory every frame.

To make the leak obvious fast, use a bigger allocation:

```cpp
char* leakedBlock = new char[1024 * 1024];  // 1 MB each frame
leakedBlock[0] = 1;
```

At 60 FPS this leaks about 60 MB/s and should quickly destabilize or crash.

Run it and observe:
- rising memory usage in Task Manager / Activity Monitor / `top`
- stutter, eventual crash or OS kill

## 5. Fix the leak (good step)

Remove the leak code completely, or if you must keep temporary allocations:

```cpp
char* temp = new char[1024 * 1024];
temp[0] = 1;
delete[] temp;
```

Then run again and confirm memory remains stable.

## 6. Lesson outcome

You now understand:
- references for guaranteed objects
- pointers for optional/dynamic objects
- why manual cleanup is error-prone
- how one ownership mistake can crash a real-time game
