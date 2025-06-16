#include "pch.h"

#include <LuaJIT/lua.hpp>
#include <raylib/raylib.h>
#include <entt/entt.hpp>

#include <filesystem> // for path

#include "Utility.h"

#include "Components.h"
#include "Systems.h"

#include "LuaWrapper.h"
#include "LuaBindings.h"
#include "RaylibLuaBindings.h"


struct WindowProperties
{
	std::string title;
	int width;
	int height;
	::ConfigFlags configFlags;
	int fps;
};


// Push window properties to Lua as a table. table is a copy, not a reference.
inline
void lua_pushwindowproperties(LuaWrapper& LW, const WindowProperties& props)
{
	lua_newtable(*LW);

	lua_pushstring(*LW, "title");
	lua_pushstring(*LW, props.title.c_str());
	lua_settable(*LW, -3); // Set the title in the table

	lua_pushstring(*LW, "width");
	lua_pushinteger(*LW, props.width);
	lua_settable(*LW, -3); // Set the width in the table

	lua_pushstring(*LW, "height");
	lua_pushinteger(*LW, props.height);
	lua_settable(*LW, -3); // Set the height in the table

	lua_pushstring(*LW, "configFlags");
	lua_pushinteger(*LW, props.configFlags);
	lua_settable(*LW, -3); // Set the configFlags in the table

	lua_pushstring(*LW, "fps");
	lua_pushinteger(*LW, props.fps);
	lua_settable(*LW, -3);

	lua_setglobal(*LW, "windowProperties"); // Set the table as a global variable

	// debug: print out the table created by get window properties
	luaL_dostring(*LW, R"(
            print("-------------");
            print("Window Properties");
			for k, v in pairs(windowProperties) do 
				print(k, v)
			end)
            print("-------------"))");
}

// anonymous namespace
namespace
{
	// ==| Raylib helpers |==

	// Initialize raylib
	void InitRaylib(const WindowProperties& wp)
	{
		::SetConfigFlags(wp.configFlags);
		::InitWindow(wp.width, wp.height, wp.title.c_str());
		::SetTargetFPS(wp.fps);
	}

	void AddPlatform(lua_State* L, entt::entity platform, entt::registry& registry,
		const Position& position, const Size& size)
	{
		registry.emplace<Position>(platform, position.x, position.y);
		registry.emplace<Size>(platform, size.width, size.height);
		registry.emplace<Terrain>(platform);
		// Optionally add a texture component if needed:
		// registry.emplace<TextureComponent>(platform, LoadTexture("path/to/texture.png"));
	};

	void InitEntities(lua_State* L, entt::entity player, entt::registry& registry)
	{
		// create a player entity with components
		[](lua_State* L, entt::entity player, entt::registry& registry)
			{
				// create a player entity with components
				//registry.emplace<Rectangle>(player, Rectangle{32.0f, 48.0f, 100.0f, 100.f });
				registry.emplace<Position>(player, 100.0f, 100.0f);
				registry.emplace<Size>(player, 32.0f, 48.0f);
				registry.emplace<Velocity>(player, 0.0f, 0.0f);
				registry.emplace<PlayerController>(player);
				// add a texture component for the player
				// registry.emplace<Sprite>(player, "assets/donkey_tilesheet_01.png");
			}(L, player, registry);

		//// add a texture component for the player
		//registry.emplace<Sprite>(player, "assets/donkey_tilesheet_01.png");

		// some terrain
		std::vector<Rectangle> platforms =
		{
			{0, 500, 800, 100},
			{200, 400, 250, 20},
			{500, 300, 200, 20}
		};

		// entities for each platform
		for (const auto& p : platforms)
		{
			auto terrain = registry.create();
			registry.emplace<Position>(terrain, p.x, p.y);
			registry.emplace<Size>(terrain, p.width, p.height);
			registry.emplace<Terrain>(terrain);
		}
	}
}

static int l_AddPlatform(lua_State* L)
{
	// 1. fetch the registry from Lua
	lua_getfield(L, LUA_REGISTRYINDEX, REGISTRY_KEY);
	entt::registry* registry = static_cast<entt::registry*>(lua_touserdata(L, -1));
	lua_pop(L, 1);

	// 2. get the platform position and size from Lua
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float width = (float)luaL_checknumber(L, 3);
	float height = (float)luaL_checknumber(L, 4);

	// 3. create a new platform entity
	entt::entity platform = registry->create();
	AddPlatform(L, platform, *registry, { x, y }, { width, height });

	// 4 . return the platform entity ID to Lua
	lua_pushinteger(L, (int)platform);

	return 1;
}

// array of functions to register with Lua
static const luaL_Reg updateFunctions[] =
{
	{"is_key_down", l_is_key_down},
	{"is_key_pressed", l_is_key_pressed},
	{"get_velocity", l_get_velocity},
	{"set_velocity", l_set_velocity},
	{"get_grounded_status", l_get_grounded_status},
	{"set_grounded_status", l_set_grounded_status},
	{"add_platform", l_AddPlatform},
	{NULL, NULL} // marking the end of the array
};

inline 
void lua_pushrectangle(lua_State* L, const Rectangle& rect)
{
	lua_newtable(L);
	lua_pushnumber(L, rect.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, rect.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, rect.width);
	lua_setfield(L, -2, "width");
	lua_pushnumber(L, rect.height);
	lua_setfield(L, -2, "height");
}
inline
void lua_pushtexture(lua_State* L, const Texture2D& texture)
{
	lua_newtable(L);
	lua_pushinteger(L, texture.id);
	lua_setfield(L, -2, "id");
	lua_pushinteger(L, texture.width);
	lua_setfield(L, -2, "width");
	lua_pushinteger(L, texture.height);
	lua_setfield(L, -2, "height");
}

/*
* Lua bindings
*/
static int l_GetWindowRect(lua_State* L)
{
	Vector2 dpi = ::GetWindowScaleDPI(); // Ensure DPI scaling is applied
	Rectangle rect = {
		::GetWindowPosition().x / dpi.x, ::GetWindowPosition().y / dpi.y,
		static_cast<float>(::GetScreenWidth()), static_cast<float>(::GetScreenHeight())
	};

	lua_pushrectangle(L, rect); // Push the rectangle as a table

	return 1; // Number of return values
}

// push delta time into lua
static int l_GetDelta(lua_State* L)
{
	lua_pushnumber(L, ::GetFrameTime()); // Push the delta time as a number
	return 1; // Number of return values
}

static constexpr luaL_Reg g_RaylibFunctions[] = {
	{"GetWindowRect", l_GetWindowRect},
	{"GetDelta", l_GetDelta},
	{NULL, NULL}
};

void RegisterFunctions(LuaWrapper& LW)
{
	// register raylib functions to Lua
	luaL_openlib(*LW, "update", updateFunctions, 0);
	luaL_openlib(*LW, "raylib", g_RaylibFunctions, 0);
}



void BindRaylibKeyCodes(LuaWrapper& LW)
{
	// debug: try to use the LuaWrapper PushTable template function
	LW.PushTable("keys", 0,
		Field{ "SPACE",  (int)KEY_SPACE },
		Field{ "P" ,     (int)KEY_P },
		Field{ "ESCAPE", (int)KEY_ESCAPE },
		Field{ "RIGHT",  (int)KEY_RIGHT },
		Field{ "LEFT",   (int)KEY_LEFT },
		Field{ "A",     (int)KEY_A },
		Field{ "D",     (int)KEY_D }
		);
	// debug: print out the key codes in Lua
	LW.DoString(R"(
        print("Input Key Codes:");
        for k, v in pairs(input) do
            print(k, v)
        end
    )");


}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::cout << LUA_VERSION << "\n";

	LuaWrapper LW("scripts/");
	LW.StartConsole(); // start the JIT console for debugging


	WindowProperties wp{ "Total Turkey Donkey", 800, 600, FLAG_WINDOW_HIGHDPI, 60 };
	lua_pushwindowproperties(LW, wp); // Push window properties to Lua	

	// create window
	InitRaylib(wp);


	// initialize Lua state and load standard lua libs TODO: put this in a lua lifetime manager later
	// store registry pointer in Lua registry for later use
	entt::registry registry;
	lua_pushlightuserdata(*LW, &registry);
	lua_setfield(*LW, LUA_REGISTRYINDEX, REGISTRY_KEY);

	RegisterFunctions(LW);

	// print out the table created by GetWindowRect to make sure it works
	LW.DoString(R"(for k, v in pairs(raylib.GetWindowRect()) do print(k, v) end)");

	BindRaylibKeyCodes(LW); // Bind raylib key codes to Lua



	// start player update script
	LW.DoFile("player_update.lua");

	// debug: load a texture and register it with Lua
	// LoadTextureTest(LW);


	// Initialize entities TODO: handle this with a config not raw
	entt::entity player = registry.create();
	InitEntities(*LW, player, registry);

	// --- MAIN GAME LOOP ---
	while (!WindowShouldClose())
	{
		float delta = ::GetFrameTime();

		// 1. handle input
		lua_getglobal(*LW, "UpdatePlayer");
		lua_pushinteger(*LW, (lua_Integer)player); // Pass player entity ID to Lua
		if (lua_pcall(*LW, 1, 0, 0) != 0)
		{
			std::cerr << "Error running UpdatePlayer: " << lua_tostring(*LW, -1) << "\n";
		}
		// don't need to pop, pcall will pop the error if it occurs


		// 2. C++ systems
		PhysicsSystem(registry, delta);
		CollisionSystem(registry);

		// 3. render
		RenderSystem(registry);
	}

	// clean up
	CloseWindow();

	return 0;
}



//// function to try to load a texture and push it into the lua environment as a table
//int LoadTextureTest(LuaWrapper& LW)
//{
//	// debug: open a texture as image, store it and set it to a rect
//	Texture2D tex = LoadTexture("assets/donkey_tilesheet_01.png");
//	if (tex.id == 0)
//	{
//		std::cerr << "Failed to load texture!" << "\n";
//		return 1;
//	}
//
//	// store the texture in the Lua registry
//	lua_pushlightuserdata(*LW, &tex);
//	lua_setfield(*LW, LUA_REGISTRYINDEX, "player_texture");
//
//	// register the texture with Lua
//	lua_newtable(*LW);
//	lua_pushstring(*LW, "id");
//	lua_pushinteger(*LW, tex.id);
//	lua_settable(*LW, -3); // Set the texture ID in the table
//	lua_pushstring(*LW, "width");
//	lua_pushinteger(*LW, tex.width);
//	lua_settable(*LW, -3); // Set the texture width in the table
//	lua_pushstring(*LW, "height");
//	lua_pushinteger(*LW, tex.height);
//	lua_settable(*LW, -3); // Set the texture height in the table
//	lua_setglobal(*LW, "texture2D"); // Set the table as a global variable
//	// register the texture with Lua as a global variable
//
//	// debug: print out the texture info in Lua
//	LW.DoString(R"(
//			print("Player Texture:", "\n\tid:", player_texture.id, "\n\twidth:",  player_texture.width, "\n\theight:", player_texture.height)
//			)");
//
//	return 0;
//}
//
//void RegisterTexture(lua_State* L, const Texture2D& texture)
//{
//	lua_newtable(L);
//	lua_pushinteger(L, texture.id);
//	lua_setfield(L, -2, "id");
//	lua_pushinteger(L, texture.width);
//	lua_setfield(L, -2, "width");
//	lua_pushinteger(L, texture.height);
//	lua_setfield(L, -2, "height");
//	lua_setglobal(L, "texture2D"); // Set the table as a global variable
//}