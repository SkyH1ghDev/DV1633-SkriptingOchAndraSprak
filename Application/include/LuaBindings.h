#pragma once

#include <LuaJIT/lua.hpp>
#include <entt/entt.hpp>
#include <raylib/raylib.h>

#include "Components.h"
#include "LuaHelpers.h"

// this key is used to store our entt::registry in the Lua registry,
// making it accessible to all our C functions called by Lua.
static const char* REGISTRY_KEY = "ENTT_REGISTRY";

// helper function to get the entt::registry from Lua's registry
inline
entt::registry* lua_get_registry(lua_State* L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, REGISTRY_KEY);
    return static_cast<entt::registry*>(lua_touserdata(L, -1));
}

// helper to register a lua module to package
inline
void register_lua_module(lua_State* L, const char* name,
                         const luaL_Reg* functions)
{
    lua_setglobal(L, name);
}


// input
static int l_is_key_down(lua_State* L)
{
    int key = g_LuaIntegerConvert(L, 1);
    lua_pushboolean(L, IsKeyDown(key));
    return 1; // Number of return values
}

static int l_is_key_pressed(lua_State* L)
{
    int key = g_LuaIntegerConvert(L, 1);
    lua_pushboolean(L, IsKeyPressed(key));
    return 1;
}


// components
static int l_get_velocity(lua_State* L)
{
    entt::registry* registry = lua_get_registry(L);

    auto entity = (entt::entity)luaL_checkinteger(L, 1);

    if (registry->valid(entity) && registry->all_of<Velocity>(entity))
    {
        const auto& vel = registry->get<Velocity>(entity);
        lua_pushnumber(L, vel.x);
        lua_pushnumber(L, vel.y);
        return 2;
    }
    return 0;
}

static int l_set_velocity(lua_State* L)
{
    entt::registry* registry = lua_get_registry(L);
	lua_pop(L, 1); // pop the registry pointer

    int argc = lua_gettop(L);
    if (argc != 3) {
        return luaL_error(L, "Expected 3 arguments: entity, vx, vy");
    }

    auto entity = (entt::entity)luaL_checkinteger(L, 1);
    float vx = g_LuaNumberConvert(L, 2);
    float vy = g_LuaNumberConvert(L, 3);

    if (registry->valid(entity) && registry->all_of<Velocity>(entity))
    {
        // if entity has a registered Velocity component, set its values
		// to what lua passed in
        auto& vel = registry->get<Velocity>(entity);
        vel.x = vx;
        vel.y = vy;
    }
    return 0;
}


static int l_get_grounded_status(lua_State* L)
{
    entt::registry* registry = lua_get_registry(L);
    auto entity = (entt::entity)luaL_checkinteger(L, 1);
    if (registry->valid(entity) && registry->all_of<PlayerController>(entity))
    {
        const auto& controller = registry->get<PlayerController>(entity);
        lua_pushboolean(L, controller.isGrounded);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}

static int l_set_grounded_status(lua_State* L)
{
    entt::registry* registry = lua_get_registry(L);
    auto entity = (entt::entity)luaL_checkinteger(L, 1);
    bool is_grounded = lua_toboolean(L, 2);

    if (registry->valid(entity) && registry->all_of<PlayerController>(entity))
    {
        auto& controller = registry->get<PlayerController>(entity);
        controller.isGrounded = is_grounded;
    }
    return 0;
}

static int l_get_jumping_status(lua_State* L)
{
    entt::registry* registry = lua_get_registry(L);
    auto entity = (entt::entity)luaL_checkinteger(L, 1);
    if (registry->valid(entity) && registry->all_of<PlayerController>(entity))
    {
        const auto& controller = registry->get<PlayerController>(entity);
        lua_pushboolean(L, controller.isJumping);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}

static int l_set_jumping_status(lua_State* L)
{
    entt::registry* registry = lua_get_registry(L);
    auto entity = (entt::entity)luaL_checkinteger(L, 1);
    bool is_jumping = lua_toboolean(L, 2);
    if (registry->valid(entity) && registry->all_of<PlayerController>(entity))
    {
        auto& controller = registry->get<PlayerController>(entity);
        controller.isJumping = is_jumping;
    }
    return 0;
}
