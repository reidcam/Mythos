#ifndef ACTOR_H
#define ACTOR_H

#include <algorithm>
#include <string>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include "glm/glm.hpp"
#include "ComponentDB.h"
#include "Collision.h"
#include "Helper.h"

struct Actor
{
public:
	// components used
	std::string actor_name = "";
	int actor_id = -1;
	bool destoryed = false;
	bool dontDestroyOnLoad = false;
	bool fromAnotherScene = false;
	std::map<std::string, Component> components; // <key, component> maybe change to a map for spatial locality
	std::map<std::string, Component> just_added_components; // <key, component>
	std::vector<std::string> components_to_remove; // <key>

	Actor(std::string actor_name, int actor_id) {}

	Actor() {}

	struct SortComponentsByKey {
		bool operator() (const std::pair<std::string, Component>& component_1, const std::pair<std::string, Component>& component_2) {
			return component_1.first < component_2.first;
		}
	};

	inline std::string GetName() const { return actor_name; }
	inline int GetID() const { return actor_id; }
	inline void InjectConvenienceReference(std::shared_ptr<luabridge::LuaRef> component_ref) { (*component_ref)["actor"] = this; }
	luabridge::LuaRef GetComponentByKey(const std::string key);
	luabridge::LuaRef GetComponent(const std::string type);
	luabridge::LuaRef GetComponents(const std::string type);
	luabridge::LuaRef AddComponent(const std::string type);
	void RemoveComponent(luabridge::LuaRef component_ref);

	void ReportError(const std::string& actor_name, const luabridge::LuaException& e);
	void OnTriggerEnter(const Collision& collision);
	void OnCollisionEnter(const Collision& collision);
	void OnTriggerExit(const Collision& collision);
	void OnCollisionExit(const Collision& collision);
	void OnDestroy();
};

#endif