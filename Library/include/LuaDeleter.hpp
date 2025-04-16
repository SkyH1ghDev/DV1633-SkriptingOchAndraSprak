#pragma once

#include <LuaJIT/lua.hpp>

template<typename T>
struct LuaDeleter 
{
    void operator()(T* ptr) const
    {
        if (ptr)
        {
            lua_close(ptr);
        }
    }
};