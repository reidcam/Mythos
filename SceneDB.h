#ifndef SCENEDB_H
#define SCENEDB_H

#include <optional>
#include <unordered_map>
#include <stdint.h>
#include <cctype>
#include "ActorTemplateDB.h"
#include <algorithm>
#include "ImageDB.h"
#include "Renderer.h"
#include "StateDB.h"

class Scene
{
public:
	inline static std::string current_scene_name = "";
	Scene() {} // default constructor

	inline static std::vector <std::shared_ptr<Actor>> actor_list;
	inline static std::vector<std::shared_ptr<Actor>> actors_to_add;
	inline static std::vector<std::shared_ptr<Actor>> actors_to_remove;


	static luabridge::LuaRef Find(const std::string name);
	static luabridge::LuaRef FindAll(const std::string name);
	static void Destroy(Actor actor);
	
	static void DontDestroyOnLoad(Actor& actor);

	bool SceneExists(const std::string& scene_name);

	void UnloadScene();

	std::string new_scene = "";
public:
	// new lua functions and vars
	struct SortOnStart
	{
		bool operator() (std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>>& component_1,
			std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>>& component_2) {
			if (component_1.first->actor_id == component_2.first->actor_id) { // check if actor id is the same
				return component_1.second.first < component_2.second.first; // return sorted by key if id is same
			}
			return component_1.first->actor_id < component_2.first->actor_id; // else return sorted by id
		}
	};
	std::vector<std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>>> onStartQueue;
	std::vector<std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>>> onUpdateQueue;
	std::vector<std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>>> onLateUpdateQueue;
	void SortOnStartQueue();
	void SortOnUpdateQueue();
	void SortOnLateUpdateQueue();
	void CallOnStart();
	void CallOnUpdate();
	void CallOnLateUpdate();
	void ReportError(const std::string& actor_name, const luabridge::LuaException& e);
	void AddNewComponentLifeCycleFunctions();
	void RemoveComponentLifeCycleFunctions();
	void AddActorsToList();
	void RemoveActorsFromList();
};

#endif