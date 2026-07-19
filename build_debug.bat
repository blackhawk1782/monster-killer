@echo off
REM Monster Killer DEBUG Build Script for MSYS2 + raylib
REM raylib installed at: C:\raylib
REM MSYS2 installed at: G:\Games\developed\engine

setlocal EnableDelayedExpansion

REM --- Configuration ---
set "RAYLIB_DIR=C:\raylib"
set "RAYLIB_SRC=%RAYLIB_DIR%\raylib\src"
set "RAYLIB_LIB=%RAYLIB_DIR%\raylib\src"
set "MSYS2_DIR=G:\Games\developed\engine"
set "MINGW_BIN=%MSYS2_DIR%\mingw64\bin"

REM --- Add MSYS2 MinGW to PATH ---
set "PATH=%MINGW_BIN%;%PATH%"

REM --- Verify gcc exists ---
where gcc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ERROR: gcc not found in PATH.
    echo Tried: %MINGW_BIN%
    echo Make sure MSYS2 MinGW is installed at %MSYS2_DIR%
    exit /b 1
)

for /f "tokens=*" %%a in ('gcc --version 2^>^&1 ^| findstr /i "gcc"') do (
    echo Found: %%a
)

REM --- Compiler settings ---
set "CC=gcc"
set "CFLAGS=-std=c99 -Wall -Wextra -g -O0 -I. -Isrc -I%RAYLIB_SRC% -DMK_DEBUG"
set "LDFLAGS=-L%RAYLIB_LIB% -lraylib -lopengl32 -lgdi32 -lwinmm"

set "EXE=MonsterKiller_debug.exe"
set "BUILD_DIR=build_debug"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM --- Collect all source files ---
set "SOURCES="
for /r "src" %%f in (*.c) do (
    set "SOURCES=!SOURCES! "%%f""
)

if "!SOURCES!"=="" (
    echo ERROR: No .c source files found in src\
    exit /b 1
)

echo.
echo === Monster Killer DEBUG Build ===
echo Compiler: %CC%
echo Raylib:   %RAYLIB_DIR%
echo MSYS2:    %MSYS2_DIR%
echo Output:   %BUILD_DIR%\%EXE%
echo.

REM --- Compile ---
echo Running: %CC% %CFLAGS% [sources] %LDFLAGS% -o %BUILD_DIR%\%EXE%
%CC% %CFLAGS% %SOURCES% %LDFLAGS% -o "%BUILD_DIR%\%EXE%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo BUILD FAILED with error %ERRORLEVEL%
    exit /b 1
)

echo.
echo DEBUG BUILD SUCCESSFUL: %BUILD_DIR%\%EXE%
echo.

REM --- Copy assets to build directory ---
if exist assets (
    xcopy /E /I /Y assets "%BUILD_DIR%\assets" > nul
    echo Assets copied.
)

echo Run with: %BUILD_DIR%\%EXE%
endlocal
