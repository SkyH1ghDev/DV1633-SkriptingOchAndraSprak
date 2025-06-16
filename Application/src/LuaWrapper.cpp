#include "pch.h"
#include "LuaWrapper.h"

#include <io.h>   // for _isatty, which checks if stdout is a console
#include <thread> // for creating a separate thread

namespace
{
    bool s_hasConsole()
    {
        return _isatty(_fileno(stdout)) != 0; // Check if stdout is a console 
    }
}

LuaWrapper::LuaWrapper(std::string scripts_path): m_scriptsPath(std::move(scripts_path))
{
    m_L = luaL_newstate();
    luaL_openlibs(m_L);

    std::cout << "[C++] Lua wrapper initiated" << "\n";
}

LuaWrapper::~LuaWrapper()
{
    lua_close(m_L);

    std::cout << "[C++] Lua wrapper closed" << "\n";
}

lua_State* LuaWrapper::operator*() const
{
    return m_L;
}

void LuaWrapper::LuaPrint(const std::string & message) const
{
    std::stringstream iss;
    iss << "print('[Lua] " << message << "')";

    if (luaL_dostring(m_L, iss.str().c_str()) != LUA_OK)
    {
        DumpError(m_L);
    }
}

void LuaWrapper::DoString(const std::string & script) const
{
    if (luaL_dostring(m_L, script.c_str()) != LUA_OK)
    {
        DumpError(m_L);
    }
}

void LuaWrapper::DoFile(const std::string & file_name) const
{
    std::stringstream iss;
    iss << m_scriptsPath << file_name;

    if (luaL_dofile(m_L, iss.str().c_str()) != LUA_OK)
    {
        DumpError(m_L);
    }
}

lua_State* LuaWrapper::GetState() const
{
    return m_L;
}

void LuaWrapper::SetScriptsPath(const std::string & scriptsPath)
{
    m_scriptsPath = scriptsPath;
}

void LuaWrapper::DumpError(lua_State* L)
{
    if (L == nullptr)
    {
        std::cerr << "Lua state is null, cannot dump error." << "\n";
        return;
    }
    if (lua_gettop(L) && lua_isstring(L, -1))
    {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1); // remove error message from stack
    }
}

void LuaWrapper::StartConsole()
{
    static std::thread consoleThread(ConsoleThreadFunction, m_L);

    consoleThread.detach(); // detach the thread to run independently
    std::cout << "[C++] Lua console started" << "\n";
}

void LuaWrapper::PushNil()const
{
    lua_pushnil(m_L);
}

void LuaWrapper::PushInteger(int value)const
{
    lua_pushinteger(m_L, value);
}

void LuaWrapper::PushEnum(unsigned int value) const
{
	lua_pushinteger(m_L, static_cast<lua_Integer>(value)); // Push as integer
}

void LuaWrapper::PushNumber(float value) const
{
    lua_pushnumber(m_L, value);
}

void LuaWrapper::PushString(const char* value) const
{
	if (value == nullptr)
	{
		lua_pushnil(m_L); // Push nil if the string is null
	}
	else
	{
		lua_pushstring(m_L, value);
	}
}

void LuaWrapper::PushBoolean(bool value)const
{
    lua_pushboolean(m_L, value);
}

void LuaWrapper::PushLightUserData(void* value)const
{
    lua_pushlightuserdata(m_L, value);
}

void LuaWrapper::PushCFunction(lua_CFunction func)const
{
    lua_pushcfunction(m_L, func);
}

void LuaWrapper::NewTable()const
{
    lua_newtable(m_L);
}

/**
 * Simple syntactic sugar to setGlobal after defining the parts of a table.
 * @param name name of the global
 */
void LuaWrapper::EndTable(const std::string& name, int index) const
{
    if (index != 0)
    {
        // table isn't global, so treat it as a field
		lua_setfield(m_L, index, name.c_str());
        return;
    }
    // no index means it's a global table
    SetGlobal(name);
}

void LuaWrapper::SetTable(int index) const
{
	lua_settable(m_L, index);
}

void LuaWrapper::GetGlobal(const std::string & name)const
{
    lua_getglobal(m_L, name.c_str());
}

void LuaWrapper::SetGlobal(const std::string & name)const
{
    lua_setglobal(m_L, name.c_str());
}

void LuaWrapper::SetField(int index, const std::string & key)const
{
    lua_setfield(m_L, index, key.c_str());
}

void LuaWrapper::GetField(int index, const std::string & key)const
{
    lua_getfield(m_L, index, key.c_str());
}

void LuaWrapper::GetTable(int index, const std::string& key) const
{
	lua_gettable(m_L, index);
}

void LuaWrapper::Pop(int count)const
{
    lua_pop(m_L, count);
}

void LuaWrapper::PushValue(int index)const
{
    lua_pushvalue(m_L, index);
}

void LuaWrapper::ConsoleThreadFunction(lua_State* L)
{
    std::string input;
    while (s_hasConsole() && L != nullptr)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit")
        {
            luaL_dostring(L, "os.exit()");
            std::cout << "[C++] Lua console closed" << "\n";
        }

        if (luaL_dostring(L, input.c_str()) != LUA_OK)
        {
            LuaWrapper::DumpError(L);
        }
    }
}
