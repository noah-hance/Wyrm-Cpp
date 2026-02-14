#include "App.h"

int main()
{
    // `main` is where the program starts.
    // Keep this file tiny so all game behavior lives in App.cpp.
    constexpr int windowWidth = 960;
    constexpr int windowHeight = 540;

    // Create the app and pass window configuration.
    HardestGame::App app(windowWidth, windowHeight, "World's Hardest Game Tutorial");

    // Run the game loop.
    app.Run();

    // Return 0 to tell the OS the program ended successfully.
    return 0;
}
