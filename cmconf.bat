@echo off
set SOURCE_DIR=.
set BUILD_DIR=build

if "%1"=="build" (
    cmake --build %BUILD_DIR%
) else (
    cmake --build %BUILD_DIR% --target clean
    cmake --preset=default -S %SOURCE_DIR% -B %BUILD_DIR%
    cmake --build %BUILD_DIR%
)