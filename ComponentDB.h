#ifndef COMPONENTDB_H
#define COMPONENTDB_H

//#include "Component.h"
//#include "StateDB.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <filesystem>
#include "Rigidbody.h"

class ComponentDB {

public:
	static std::unordered_map<std::string, std::shared_ptr<luabridge::LuaRef>> component_tables;


	static void CppLog(const std::string& message);
	static void CppLogError(const std::string& message);

	static void InitComponent(const std::string& type);
	static void ComponentExists(const std::string& type);

	static void SetComponentLifecylceVariables(Component& component);

	static void EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table);

	static Component CreateRigidbody();
private:
};

#endif