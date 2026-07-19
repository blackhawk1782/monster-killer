@echo off
REM Clean all build artifacts

echo Cleaning build directories...
if exist build_msys rmdir /S /Q build_msys
if exist build_debug rmdir /S /Q build_debug
if exist build rmdir /S /Q build
echo Done.
