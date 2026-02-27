cmake_minimum_required(VERSION 3.20)

# Project metadata shown by IDEs and generated solutions.
project(WyrmCpp
    VERSION 0.1.0
    DESCRIPTION "Starter tutorial project: build a simple World's Hardest Game-inspired scene with raylib"
    LANGUAGES C CXX
)

# Use modern C++ and keep requirements explicit for new learners.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Helps keep generated files organized in one folder.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Prefer an existing installed raylib package.
# If it is not available, automatically download raylib with FetchContent.
find_package(raylib CONFIG QUIET)

if(NOT raylib_FOUND)
    include(FetchContent)

    # Pull raylib from the official repository when configuring the project.
    # If this tag is old by the time you read this, update it in one place here.
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )

    # Prevent raylib examples/tools from cluttering tutorial builds.
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(raylib)
endif()

# The tutorial executable.
add_executable(WyrmCpp
    src/main.cpp
    src/App.cpp
    include/App.h
)

target_include_directories(WyrmCpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(WyrmCpp
    PRIVATE
        raylib
)

# Nice project organization in Visual Studio and some other IDEs.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    src/main.cpp
    src/App.cpp
    include/App.h
)

# Ensure Visual Studio opens this tutorial executable by default.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT WyrmCpp)
