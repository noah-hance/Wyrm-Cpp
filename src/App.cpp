#include "App.h"
#include <chrono>
#include <ctime>
#include <iostream>
using namespace std;

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
        , _tailLength(2)
        , _tailStart()
        , _target {5.0f, 5.0f, 20.0f, 20.0f}
        , _targetPosition(Vector2 {0, 0})
        , _board()
        , _background {50.0f, 85.0f, 900.0f, 900.0f}
        , _pointCount(std::make_unique<int>(0))
        , _deathCount(std::make_unique<int>(0))
        , _maxPointCount(std::make_unique<int>(0))
        , _stepsTaken(std::make_unique<int>(0))
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
        Rectangle& target = _target;

        _tailStart = new TailSection(
            ConvertPosition(_boardDimension, _playerPosition.x, _playerPosition.y),
            _tailLength);

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
        ++(*_stepsTaken);
    }

    void App::Draw() const
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
        DrawText(
            TextFormat("Max points: %d", *_maxPointCount),
            static_cast<float>(_windowWidth) - 200.0f,
            20,
            24,
            GREEN);
        // DrawText(TextFormat("Points: %f", _playerPosition.x), 20, 20, 24, GREEN);
    }

    void App::DrawWyrm() const
    {
        Color tailColor = Color {0, 228, 48, 155};
        for (int i = 0; i < _boardDimension * _boardDimension; i++)
        {
            int tailValue = _board[i];
            if (tailValue < 0)
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

    void App::SetPlayerPosition(Vector2 position)
    {
        _playerPosition.x = position.x;
        _playerPosition.y = position.y;

        _player.x = 53.5 + _playerPosition.x * 24.305555f;
        _player.y = 87.5 + _playerPosition.y * 24.305555f;
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
            // since goal reached, only update tailStart position
            Goal();
            TailSection* newSection = new TailSection(
                ConvertPosition(_boardDimension, _playerPosition.x, _playerPosition.y),
                _tailLength);
            newSection->next = _tailStart;

            _tailStart = newSection;
            PrintTail(_tailStart);
        }
        else
        {
            // UpdateTail();
        }
    }

    void App::PrintTail(TailSection* start)
    {
        TailSection* ts = start;
        while (ts != NULL)
        {
            cout << ts->position << ": ";
            cout << ts->countdownValue << endl;
            ts = ts->next;
        }
        cout << endl;
    }

    /*void App::UpdateTail()
    {
        TailSection ts = _tailStart;
        cout << "tail: " << endl;

        while (&ts != NULL)
        {

            ts = *ts.next;
        }
    }*/

    void App::Reset()
    {
        ResetBoard();
        SetPlayerPosition(Vector2 {18, 0});
        _board[18] = -1 * _tailLength + 1; // resets position underneath spawn
        _moveDirection = Vector2 {0, 0};

        ++(*_deathCount);
        if (*_maxPointCount < *_pointCount)
        {
            *_maxPointCount = *_pointCount;
        }
        *_pointCount = 0;
        ChangeGoalPosition();
    }

    void App::Goal()
    {
        ++(*_pointCount);
        _tailLength++;
        ChangeGoalPosition();
        cout << "Points: " << *_pointCount << endl;
    }

    void App::ChangeGoalPosition()
    {
        int prevBoardPosition =
            ConvertPosition(_boardDimension, _targetPosition.x, _targetPosition.y);
        _board[prevBoardPosition] = 0;

        int newGoalPositionX = BoundedRand(0, _boardDimension);
        int newGoalPositionY = BoundedRand(0, _boardDimension);

        int boardPosition = ConvertPosition(_boardDimension, newGoalPositionX, newGoalPositionY);
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
        SetPlayerPosition(position);

        int boardPosition = ConvertPosition(_boardDimension, position.x, position.y);
        int status = _board[boardPosition];

        _board[boardPosition] = -1 * _tailLength + 1;
        return status;
    }

    void App::ResetBoard()
    {
        srand(*_deathCount * *_stepsTaken + 200); // set new seed based on movement and deaths

        for (int i = 0; i < _boardDimension * _boardDimension; i++)
        {
            _board[i] = 0;
        }
        // DeleteTail(_tailStart->next);
        //  cout << "Tail: " << _tailStart.countdownValue << endl;

        _tailLength = 2;
        TailSection* newSection = new TailSection(
            ConvertPosition(_boardDimension, _playerPosition.x, _playerPosition.y),
            _tailLength);
        _tailStart = newSection;
    }

    void App::DeleteTail(TailSection* ts)
    {
        if (ts == NULL)
        {
            return;
        }
        DeleteTail(ts->next);
        delete ts;
    }

    int App::BoundedRand(int min, int max) const
    {
        if (max <= min)
            return -1;
        return min + rand() % (max);
    }

    int App::ConvertPosition(int dimension, float x, float y) const
    {
        return _boardDimension * y + x;
    }

}
