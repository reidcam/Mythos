#include "ComponentDB.h"
std::unordered_map<std::string, std::shared_ptr<luabridge::LuaRef>> ComponentDB::component_tables;

// console logs a message for the Debug API
void ComponentDB::CppLog(const std::string& message) {
	std::cout << message + "\n";
}

// console logs an error message for the Debug API
void ComponentDB::CppLogError(const std::string& message) {
	std::cerr << message + "\n";
}

void ComponentDB::EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table)
{
	/* We must create a metatable to establish inheritance in lua */
	luabridge::LuaRef new_metatable = luabridge::newTable(StateDB::lua_state);
	new_metatable["__index"] = parent_table;

	/* We must use the raw lua (-API (lua stack) to perform a "setmetatable" operation */
	instance_table.push(StateDB::lua_state);
	new_metatable.push(StateDB::lua_state);
	lua_setmetatable(StateDB::lua_state, -2);
	lua_pop(StateDB::lua_state, 1);
}

Component ComponentDB::CreateRigidbody()
{
	Component component;
	Component* pointer = new Component();

	Rigidbody* rigidbody = new Rigidbody();
	luabridge::LuaRef componentRef(StateDB::lua_state, rigidbody);
	component.componentRef = std::make_shared<luabridge::LuaRef>(componentRef);

	component.type = "Rigidbody";
	component.hasLateUpdate = false;
	component.hasUpdate = false;
	component.hasStart = true;
	(*component.componentRef)["enabled"] = true;
	*pointer = component;
	return component;
}

/// <summary>
/// intializes the specified component
/// </summary>
void ComponentDB::InitComponent(const std::string& type) {
	// make sure that the component doesn't already exist
	if (component_tables.find(type) != component_tables.end())
		return;
	
	std::filesystem::path file_path = "resources/component_types/" + type + ".lua";

	// initialize the component
	if (luaL_dofile(StateDB::lua_state, file_path.string().c_str()) != LUA_OK) {
		std::cout << "problem with lua file " << file_path.stem().string();
		exit(0);
	}

	std::string componentName = file_path.stem().string();
	component_tables.insert({ componentName,
		std::make_shared<luabridge::LuaRef>(
			luabridge::getGlobal(StateDB::lua_state, componentName.c_str()))
	});
}

/// <summary>
/// checks to make sure that the component exists and errors if it doesn't
/// </summary>
/// <param name="type"></param>
void ComponentDB::ComponentExists(const std::string& type)
{
	if (!std::filesystem::exists("resources/component_types/" + type + ".lua")) {
		std::cout << "error: failed to locate component " + type;
		exit(0);
	}
}

/// <summary>
/// Checks to see if the component has lifecycle functions and sets the corresponding variables if it does
/// </summary>
/// <param name="component"></param>
void ComponentDB::SetComponentLifecylceVariables(Component& component)
{
	if ((*component.componentRef).isTable()) {
		if ((*component.componentRef)["OnStart"].isFunction())
			component.hasStart = true;
		if ((*component.componentRef)["OnUpdate"].isFunction())
			component.hasUpdate = true;
		if ((*component.componentRef)["OnLateUpdate"].isFunction())
			component.hasLateUpdate = true;
	}
}