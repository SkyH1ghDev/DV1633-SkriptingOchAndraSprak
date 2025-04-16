#include <raylib/raylib.h>
#include <LuaDeleter.hpp>
#include <memory>
#include <limits>
#include <cstdint>
#include <iostream>

#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    InitWindow(400, 400, "Test");

    // Uncap FPS
    SetTargetFPS(std::numeric_limits<std::int32_t>::max());
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Hello World!", 100, 100, 20, LIGHTGRAY);
        }
        EndDrawing();
    }

    std::unique_ptr<lua_State, LuaDeleter<lua_State>> luaState
    (
        lua_open(),
        LuaDeleter<lua_State>()
    );

    std::cout << luaState;
    
    CloseWindow();
    return 0;
}