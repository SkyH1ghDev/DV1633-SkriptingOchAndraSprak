#include <raylib/raylib.h>
#include <LuaDeleter.hpp>
#include <memory>
#include <limits>
#include <cstdint>
#include <iostream>
#include <string>

#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

std::string GetValueString(lua_State* L, int i)
{
    switch (lua_type(L, i))
    {
        case LUA_TNIL: return "nil";
        case LUA_TBOOLEAN:
        return lua_toboolean(L, i) ? "true" : "false";
        case LUA_TNUMBER: return std::to_string(lua_tonumber(L, i));
        case LUA_TSTRING: return lua_tostring(L, i);
        default: return "";
    }
}

void DumpStack(const std::unique_ptr<lua_State, LuaDeleter<lua_State>>& luaState)
{
    int size = lua_gettop(luaState.get());
    std::cout << "--- STACK BEGIN ---" << "\n";
    for (int i = size; i > 0; i--)
    {
        std::cout << i
        << "\t"
        << lua_typename(luaState.get(), lua_type(luaState.get(), i))
        << "\t\t" << GetValueString(luaState.get(), i)
        << "\n";
    }
    std::cout << "---- STACK END ----" << "\n";
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // ------------------------------------- //
    // RAYLIB                                //
    // ------------------------------------- //
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
    
    CloseWindow();
    
    // ------------------------------------- //
    // LUA                                   //
    // ------------------------------------- //
    std::unique_ptr<lua_State, LuaDeleter<lua_State>> luaState
    (
        lua_open(),
        LuaDeleter<lua_State>()
    );

    lua_pushstring(luaState.get(), "Hello World!");
    DumpStack(luaState);

    return 0;
}