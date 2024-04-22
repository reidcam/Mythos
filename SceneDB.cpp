#include "SceneDB.h"

luabridge::LuaRef Scene::Find(const std::string name)
{
	std::shared_ptr<Actor> actor_to_return;
	actor_to_return.reset();
	luabridge::LuaRef table = luabridge::newTable(StateDB::lua_state);

	for (int i = 0; i < Scene::actor_list.size(); i++) {
		if (Scene::actor_list[i]->actor_name == name && !Scene::actor_list[i]->destoryed) {
			table = Scene::actor_list[i].get();
			actor_to_return = Scene::actor_list[i];
		}

	}

	if (actor_to_return.get() == nullptr) { // check actors to add as well
		for (int i = 0; i < Scene::actors_to_add.size(); i++) {
			if (Scene::actors_to_add[i]->actor_name == name && !Scene::actors_to_add[i]->destoryed) {
				table = Scene::actors_to_add[i].get();
				actor_to_return = Scene::actors_to_add[i];
			}
		}
	}

	if (actor_to_return.get() != nullptr)
		return table;
	else
		return luabridge::LuaRef(StateDB::lua_state);
}

luabridge::LuaRef Scene::FindAll(const std::string name)
{

	luabridge::LuaRef result = luabridge::newTable(StateDB::lua_state);
	std::vector<std::shared_ptr<Actor>> actors = Scene::actor_list;
	std::vector<std::shared_ptr<Actor>> actors_to_add = Scene::actors_to_add;

	int index = 1;
	for (auto& actor : actors)
	{
		if (actor->actor_name == name && !actor->destoryed)
		{
			result[index] = actor.get();
			index++;
		}
	}

	// reset to check actors to add
	for (auto& actor : actors_to_add) {
		if (actor->actor_name == name)
		{
			result[index] = actor.get();
			index++;
		}
	}

	return result;
}

void Scene::Destroy(Actor actor)
{
	// only need to check actor_list bc any new actors should have been already added to the scene
	for (int i = 0; i < actor_list.size(); i++) {
		if (actor_list[i]->actor_id == actor.actor_id) { // if they have the same id, then they are the same actor
			// loop through all of the actor's components and disable them and add them to the components to remove list
			for (auto& component : actor_list[i]->components) {
				(*component.second.componentRef)["enabled"] = false;
				actor_list[i]->components_to_remove.push_back(component.first);
			}

			actor_list[i]->destoryed = true;
			actors_to_remove.push_back(actor_list[i]); 
			break;
		}

	}

	for (int i = 0; i < actors_to_add.size(); i++) {
		if (actors_to_add[i]->actor_id == actor.actor_id) {
			for (auto& component : actors_to_add[i]->components) {
				(*component.second.componentRef)["enabled"] = false;
				actors_to_add[i]->components_to_remove.push_back(component.first);
			}

			actors_to_add[i]->destoryed = true;
			actors_to_remove.push_back(actor_list[i]);
			break;
		}
	}
}

void Scene::DontDestroyOnLoad(Actor& actor)
{
	for (auto& actor_pointer : actor_list) {
		if (actor_pointer->actor_id == actor.actor_id) {
			actor_pointer->dontDestroyOnLoad = true;
		}
	}
}

/// <summary>
/// Checks to see if there is a scene file with name scene_name in resources/scenes/
/// </summary>
/// <param name="scene_name"></param>
/// <returns></returns>
bool Scene::SceneExists(const std::string& scene_name)
{
	if (!std::filesystem::exists("resources/scenes/" + scene_name + ".scene")) {
		std::cout << "error: scene " + scene_name + " is missing";
		exit(0);
	}
	return true;
}

/// <summary>
/// Unloads the current scene by calling delete on every actor on the heap
/// </summary>
void Scene::UnloadScene()
{
	Scene::actor_list.clear();
	Scene::onStartQueue.clear();
	Scene::onUpdateQueue.clear();
	Scene::onLateUpdateQueue.clear();
}

/// <summary>
/// sorts the onStartQueue
/// </summary>
void Scene::SortOnStartQueue()
{
	sort(onStartQueue.begin(), onStartQueue.end(), SortOnStart());
}

void Scene::SortOnUpdateQueue()
{
	sort(onUpdateQueue.begin(), onUpdateQueue.end(), SortOnStart());
}

void Scene::SortOnLateUpdateQueue()
{
	sort(onLateUpdateQueue.begin(), onLateUpdateQueue.end(), SortOnStart());
}

/// <summary>
/// calls the OnStart functions for the components in onStartQueue
/// </summary>
void Scene::CallOnStart()
{
	SortOnStartQueue(); // make sure the queue is sorted alphabetially by name before calling
	for (auto& component : onStartQueue) {
		try {
			luabridge::LuaRef enabled = (*component.second.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.second.componentRef)["OnStart"](*component.second.second.componentRef);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(component.first->actor_name, e);
		}

	}

	// clear the queue
	onStartQueue.clear();
}

void Scene::CallOnUpdate()
{
	SortOnUpdateQueue(); // make sure the queue is sorted alphabetially by name before calling
	for (auto& component : onUpdateQueue) {
		try {
			luabridge::LuaRef enabled = (*component.second.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.second.componentRef)["OnUpdate"](*component.second.second.componentRef);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(component.first->actor_name, e);
		}
	}
}

void Scene::CallOnLateUpdate()
{
	SortOnLateUpdateQueue();
	for (auto& component : onLateUpdateQueue) {
		try {
			luabridge::LuaRef enabled = (*component.second.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.second.componentRef)["OnLateUpdate"](*component.second.second.componentRef);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(component.first->actor_name, e);
		}
	}
}

void Scene::ReportError(const std::string& actor_name, const luabridge::LuaException& e)
{
	std::string error_message = e.what();

	// Normalize file paths across platforms
	std::replace(error_message.begin(), error_message.end(), '\\', '/');

	// Display (with color codes)
	std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
}

/// <summary>
/// adds the life cycle functions of components just added to actors
/// </summary>
/// <param name="actor"></param>
void Scene::AddNewComponentLifeCycleFunctions()
{
	for (auto& actor : actor_list) { // check all actors for new components
		std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>> pair_to_be_added;
		pair_to_be_added.first = actor;

		for (auto& component : actor->just_added_components) {
			pair_to_be_added.second.first = component.first; // add key
			pair_to_be_added.second.second = component.second; // add component

			if (component.second.hasStart)
				onStartQueue.push_back(pair_to_be_added);
			if (component.second.hasUpdate)
				onUpdateQueue.push_back(pair_to_be_added);
			if (component.second.hasLateUpdate)
				onLateUpdateQueue.push_back(pair_to_be_added);
		}
		actor->just_added_components.clear();
	}
}

/// <summary>
/// removes components from life cycle functions
/// </summary>
void Scene::RemoveComponentLifeCycleFunctions()
{
	bool component_found = false;

	for (auto& actor : actor_list) {
		actor->OnDestroy(); // call on Destroy on all of the components
		for (auto& component_key : actor->components_to_remove) {
			// search all component life cycle functions for instances of the component
			for (size_t i = 0; i < onStartQueue.size();) {
				if (onStartQueue[i].second.first == component_key && onStartQueue[i].first->actor_id == actor->actor_id) {
					std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>> back_element = onStartQueue.back();
					onStartQueue[i] = back_element;
					component_found = true;
					break;
				}
				else {
					++i; // only increment if we remove an element
				}
			}
			if (!onStartQueue.empty() && component_found)
				onStartQueue.pop_back();

			component_found = false; // reset indicator variable
			for (size_t i = 0; i < onUpdateQueue.size();) {
				if (onUpdateQueue[i].second.first == component_key && onUpdateQueue[i].first->actor_id == actor->actor_id) {
					std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>> back_element = onUpdateQueue.back();
					onUpdateQueue[i] = back_element;
					component_found = true;
					break;
				}
				else {
					++i; // only increment if we remove an element
				}
			}
			if (!onUpdateQueue.empty() && component_found)
				onUpdateQueue.pop_back();

			component_found = false; // reset indicator variable
			for (size_t i = 0; i < onLateUpdateQueue.size();) {
				if (onLateUpdateQueue[i].second.first == component_key && onLateUpdateQueue[i].first->actor_id == actor->actor_id) {
					std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>> back_element = onLateUpdateQueue.back();
					onLateUpdateQueue[i] = back_element;
					component_found = true;
					break;
				}
				else {
					++i; // only increment if we remove an element
				}
			}
			if (!onLateUpdateQueue.empty() && component_found)
				onLateUpdateQueue.pop_back();

			component_found = false; // reset indicator variable
		}

		actor->components_to_remove.clear();
	}
}

/// <summary>
/// adds the actors in actors_to_add to actor_list
/// </summary>
void Scene::AddActorsToList()
{
	for (auto& actor : Scene::actors_to_add) {
		if (actor->actor_id == -1) {
			auto t = 0;
		}
		else
			actor_list.push_back(actor);
	}
	actors_to_add.clear();
}

/// <summary>
/// removes the actors in actors_to_remove from actor_list
/// </summary>
void Scene::RemoveActorsFromList()
{
	bool actor_found = false;
	for (auto& actor_to_remove : actors_to_remove) {
		for (int i = 0; i < actor_list.size(); i++) {
			if (actor_to_remove->actor_id == actor_list[i]->actor_id) { // we have found an actor to remove
				std::shared_ptr<Actor> back_actor = actor_list.back();
				actor_found = true;
				actor_list[i] = back_actor;
			}
		}
		if (!actor_list.empty() && actor_found)
			actor_list.pop_back();
		actor_found = false;
	}
	actors_to_remove.clear(); // clear the buffer
}
