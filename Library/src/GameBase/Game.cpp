#include <GameBase/Game.hpp>
#include <GameBase/GameSetup.hpp>
#include <GameBase/GameLoop.hpp>
//#include <sol/sol.hpp>
#include <print>

using namespace Library::GameBase;

void Game::Run()
{
    //lua_State* L = luaL_newstate();
    //sol::state luaState;

    int x = 0;

    //luaState.set_function("beep", [&x]{ ++x; });

    //luaState.script("beep()");

    std::print("{0}", x);

    //lua_close(L);
}
