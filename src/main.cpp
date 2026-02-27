#include "App.h"

int main()
{
    // `main` is where the program starts.
    // Keep this file tiny so all game behavior lives in App.cpp.
    constexpr int windowWidth = 1005;
    constexpr int windowHeight = 1005;

    // Create the app and pass window configuration.
    WyrmCpp::App app(windowWidth, windowHeight, "Wyrms Consumption");

    // Run the game loop.
    app.Run();

    // Return 0 to tell the OS the program ended successfully.
    return 0;
}
