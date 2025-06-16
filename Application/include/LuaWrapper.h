#pragma once

#include <string>
#include <LuaJIT/lua.hpp>

#include <initializer_list> // for variadic templates
#include <type_traits> // for type traits, specifically std::is_same_v

template <typename T>
struct Field
{
	const char* key;
	T value;

	Field(const char* key, T value) : key(key), value(std::move(value)) {}
};


// wrapping lua to set scripts location, automatically create on construct and close on destruct
class LuaWrapper
{
public:

	explicit LuaWrapper(std::string scripts_path);
	~LuaWrapper();

	// get lua state by pointer
	lua_State* operator*() const;
	lua_State* GetState() const;

	// helper functions
	void LuaPrint(const std::string& message) const;
	void DoString(const std::string& script) const;
	void DoFile(const std::string& file_name) const;

	// set path for dofile
	void SetScriptsPath(const std::string& scriptsPath);

	void StartConsole();

	// lua stack function wrappers
	void PushNil() const;
	void PushInteger(int value) const;
	void PushEnum(unsigned int value) const; // for enum values
	void PushNumber(float value) const;
	void PushString(const char* value) const;
	void PushBoolean(bool value) const;
	void PushLightUserData(void* value) const;
	void PushCFunction(lua_CFunction func) const;

	void NewTable() const;
	void EndTable(const std::string& name, int index) const;
	void GetGlobal(const std::string& name) const;
	void GetField(int index, const std::string& key) const;
	void GetTable(int index, const std::string& key) const;
	void SetField(int index, const std::string& key) const;
	void SetTable(int index) const;
	void SetGlobal(const std::string& name) const;
	void Pop(int count = 1) const;
	void PushValue(int index) const;


	static void DumpError(lua_State* L);

	// this is explicit, no copies or moving
	LuaWrapper(const LuaWrapper& other) = delete;
	LuaWrapper(LuaWrapper&& other) noexcept = delete;
	LuaWrapper& operator=(const LuaWrapper& other) = delete;
	LuaWrapper& operator=(LuaWrapper&& other) noexcept = delete;
private:
	lua_State* m_L;
	std::string m_scriptsPath;

	// function to run in a separate thread for console input, allowing Lua commands to be executed
	// during runtime without blocking the main application loop.
	static void ConsoleThreadFunction(lua_State* L);


public:
	// experimental!

	template <typename Type>
	void PushType(const Type& value) const
	{
		if constexpr (std::is_same_v<Type, int>)
		{
			PushInteger(value);
		}
		else if constexpr (std::is_same_v<Type, float>)
		{
			PushNumber(value);
		}
		else if constexpr (std::is_same_v<Type, const char*>)
		{
			PushString(value);
		}
		else if constexpr (std::is_same_v<Type, std::string>)
		{
			PushString(value.c_str());
		}
		else if constexpr (std::is_same_v<Type, bool>)
		{
			PushBoolean(value);
		}
		else if constexpr (std::is_same_v<Type, void*>)
		{
			PushLightUserData(value);
		}
		else if constexpr (std::is_same_v<Type, lua_CFunction>)
		{
			PushCFunction(value);
		}
		else if constexpr (std::is_same_v<Type, unsigned int>)
		{
			PushEnum(value); // for enum values
		}
		else if constexpr (std::is_same_v<Type, std::nullptr_t>)
		{
			PushNil(); // push nil for nullptr
		}
		else
		{
			static_assert(std::is_same_v<Type, void>, "Unsupported type for Lua push");
		}
	}


	template <typename T>
	void PushField(const Field<T>& field)
	{
		PushType(field.value);
		SetField(-2, field.key); // -2 because the table is at -1
	}


	template<typename ...Fields>
	void PushTable(const char* title, int index, Fields... fields)
	{
		NewTable();
		(PushField(fields), ...);// fold expression. for each field in fields -> PushField(field)
		EndTable(title, index);
	}

};
