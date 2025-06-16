#pragma once

#include <LuaJIT/lua.hpp>

// Lua helpers

/**
 * 
 * @param L 
 * @param numArg
 * @return integer
 * Verify and return an integer from the stack
 */
inline
int g_LuaIntegerConvert(lua_State* L, int numArg)
{
	return static_cast<int>(luaL_checkinteger(L, numArg));
}

inline
float g_LuaNumberConvert(lua_State* L, int numArg)
{
	return static_cast<float>(luaL_checknumber(L, numArg));
}

inline
unsigned char g_LuaByteConvert(lua_State* L, int numArg)
{
	return static_cast<unsigned char>(luaL_checkinteger(L, numArg));
}

inline
bool g_LuaBooleanConvert(lua_State* L, int numArg)
{
	return lua_toboolean(L, numArg) != 0;
}

inline
const char* g_LuaStringConvert(lua_State* L, int numArg)
{
	return luaL_checkstring(L, numArg);
}

inline
void* g_LuaLightUserDataConvert(lua_State* L, int numArg)
{
	return lua_touserdata(L, numArg);
}

inline
lua_CFunction g_LuaCFunctionConvert(lua_State* L, int numArg)
{
	if (!lua_isfunction(L, numArg))
	{
		luaL_error(L, "Argument is not a function");
		return nullptr;
	}
	return lua_tocfunction(L, numArg);
}

// check
inline
bool g_LuaIsNil(lua_State* L, int index)
{
	return lua_isnil(L, index) != 0;
}