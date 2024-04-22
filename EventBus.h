#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <unordered_map>
#include "StateDB.h"

class EventBus
{
public:
	// event_name, vector<table, function>
	std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> subcriptions;
	std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> forbidden_subcriptions;

	static void Publish(const std::string& event_type, const luabridge::LuaRef& event_object = luabridge::LuaRef(StateDB::lua_state));
	static void Subscribe(const std::string& event_type, const luabridge::LuaRef& table, const luabridge::LuaRef& function);
	static void Unsubscribe(const std::string& event_type, const luabridge::LuaRef& table, const luabridge::LuaRef& function);
};

#endif