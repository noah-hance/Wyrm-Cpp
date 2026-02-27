@echo off
setlocal

REM ------------------------------------------------------------
REM generate_vs2022.bat
REM ------------------------------------------------------------
REM This script creates a Visual Studio 2022 solution in /build.
REM Use it from the repository root:
REM   generate_vs2022.bat
REM ------------------------------------------------------------

set BUILD_DIR=build

if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

echo [1/2] Configuring CMake project for Visual Studio 2022...
cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -S . -B "%BUILD_DIR%" -G "Visual Studio 17 2022"
if errorlevel 1 (
    echo.
    echo CMake configure failed.
    exit /b 1
)

echo [2/2] Done.
echo.
echo Open "%BUILD_DIR%\WyrmCpp.sln" in Visual Studio.

endlocal
