cmake_minimum_required(VERSION 3.16)
project(MonsterKiller C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable all warnings
if(MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()

# Find raylib
find_package(raylib 5.0 QUIET)
if(NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Source files
set(SOURCES
    src/main.c
    src/engine/engine.c
    src/engine/config.c
    src/raycaster/raycaster.c
    src/renderer/renderer.c
    src/renderer/texture_manager.c
    src/camera/camera.c
    src/camera/rail_path.c
    src/map/map.c
    src/map/map_loader.c
    src/sprites/sprites.c
    src/sprites/sprite_atlas.c
    src/animation/animation.c
    src/player/player.c
    src/player/aiming.c
    src/weapons/weapon.c
    src/weapons/weapon_data.c
    src/enemy/enemy.c
    src/enemy/enemy_ai.c
    src/enemy/enemy_states.c
    src/boss/boss.c
    src/boss/boss_ai.c
    src/physics/collision.c
    src/particles/particles.c
    src/particles/blood.c
    src/audio/audio_manager.c
    src/audio/sound_manager.c
    src/ui/ui.c
    src/ui/hud.c
    src/ui/menu.c
    src/events/event_system.c
    src/events/triggers.c
    src/save/savegame.c
    src/utilities/math_utils.c
    src/utilities/memory_pool.c
    src/utilities/string_utils.c
    src/utilities/random.c
)

# Header include directories
include_directories(src)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Windows-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Set subsystem for Windows release builds (optional)
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
