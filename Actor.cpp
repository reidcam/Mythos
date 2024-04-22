#include "Actor.h"

luabridge::LuaRef Actor::GetComponentByKey(const std::string key) {
	if (components.find(key) != components.end())
		return (*components[key].componentRef); // return the lua ref
	else
		return luabridge::LuaRef(StateDB::lua_state);
}

// rework to work like get components
luabridge::LuaRef Actor::GetComponent(const std::string type) {
	std::vector<std::pair<std::string, Component>> name_and_component;

	// find the components
	for (auto& component : components) {
		if (component.second.type == type) {
			name_and_component.push_back(std::pair<std::string, Component>(component.first, component.second));
		}

	}

	sort(name_and_component.begin(), name_and_component.end(), SortComponentsByKey());


	if (name_and_component.size() != 0) {
		if ((*name_and_component[0].second.componentRef)["enabled"].isBool() &&
			(*name_and_component[0].second.componentRef)["enabled"].cast<bool>() == false) { // check to see if the component is disabled
			return luabridge::LuaRef(StateDB::lua_state);
		}
		else {
			return *name_and_component[0].second.componentRef.get();;
		}

	}
	return luabridge::LuaRef(StateDB::lua_state);
}

luabridge::LuaRef Actor::GetComponents(const std::string type)
{
	std::vector<std::pair<std::string, Component>> name_and_component;
	luabridge::LuaRef table = luabridge::newTable(StateDB::lua_state);

	// find the components
	int i = 1;
	for (auto& component : components) {
		if (component.second.type == type) {
			table[i] = *(component.second.componentRef);
			i++;
		}
	}

	return table;
}

luabridge::LuaRef Actor::AddComponent(const std::string type)
{
	// create the key: current_frame * number_of_times_AddComponent_has_been_called
	std::string key = "r" + std::to_string(StateDB::added_component_count);
	StateDB::added_component_count++; // must be incremented every time add component is called
	Component component_to_insert;

	if (type != "Rigidbody") {
		// make sure the component exists before trying to add it
		ComponentDB::ComponentExists(type);
		ComponentDB::InitComponent(type);

		// create component
		component_to_insert = Component();
		component_to_insert.componentRef = std::make_shared<luabridge::LuaRef>(luabridge::newTable(StateDB::lua_state));
		component_to_insert.type = type;

		// add template to component table
		ComponentDB::EstablishInheritance(*component_to_insert.componentRef,
			*ComponentDB::component_tables.at(type));

		// set lua ref table variables
		ComponentDB::SetComponentLifecylceVariables(component_to_insert);
	}
	else {
		component_to_insert = ComponentDB::CreateRigidbody();
	}


	(*component_to_insert.componentRef)["key"] = key;
	(*component_to_insert.componentRef)["type"] = type;
	(*component_to_insert.componentRef)["enabled"] = true; // how to set based on the template?????

	// insert component into actor's map and just_added_components
	components.insert(std::pair<std::string, Component>(key, component_to_insert));
	just_added_components.insert(std::pair<std::string, Component>(key, component_to_insert));

	return *component_to_insert.componentRef;
}

void Actor::RemoveComponent(luabridge::LuaRef component_ref) {
	for (auto& component : components) {
		if (*component.second.componentRef == component_ref) {
			component_ref["enabled"] = false; // get the reference from the component
			components_to_remove.push_back(component.first); // add the key to be removed
			break;
		}
	}
}

void Actor::ReportError(const std::string& actor_name, const luabridge::LuaException& e)
{
	std::string error_message = e.what();

	// Normalize file paths across platforms
	std::replace(error_message.begin(), error_message.end(), '\\', '/');

	// Display (with color codes)
	std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
}

void Actor::OnTriggerEnter(const Collision& collision)
{
	std::vector<std::pair<std::string, Component>> name_and_component;

	// find all components on the actor that have OnTriggerEnter functions
	for (auto& component : components) {
		if ((*component.second.componentRef)["OnTriggerEnter"].isFunction())
			name_and_component.push_back(std::pair<std::string, Component>(component.first, component.second));
	}

	sort(name_and_component.begin(), name_and_component.end(), SortComponentsByKey());

	// call all of the OnTriggerEnter functions in order by key
	for (auto& component : name_and_component) {
		try {
			luabridge::LuaRef enabled = (*component.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.componentRef)["OnTriggerEnter"](*component.second.componentRef, collision);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(actor_name, e);
		}
	}
}

void Actor::OnCollisionEnter(const Collision& collision)
{
	std::vector<std::pair<std::string, Component>> name_and_component;

	// find all components on the actor that have OnCollisionEnter functions
	for (auto& component : components) {
		
		if ((*component.second.componentRef)["OnCollisionEnter"].isFunction()) {
			name_and_component.push_back(std::pair<std::string, Component>(component.first, component.second));
		}
	}

	sort(name_and_component.begin(), name_and_component.end(), SortComponentsByKey());

	// call all of the OnTriggerEnter functions in order by key
	for (auto& component : name_and_component) {
		try {
			luabridge::LuaRef enabled = (*component.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.componentRef)["OnCollisionEnter"](*component.second.componentRef, collision);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(actor_name, e);
		}
	}
}

void Actor::OnTriggerExit(const Collision& collision)
{
	std::vector<std::pair<std::string, Component>> name_and_component;

	// find all components on the actor that have OnTriggerExit functions
	for (auto& component : components) {
		if ((*component.second.componentRef)["OnTriggerExit"].isFunction())
			name_and_component.push_back(std::pair<std::string, Component>(component.first, component.second));
	}

	sort(name_and_component.begin(), name_and_component.end(), SortComponentsByKey());

	// call all of the OnTriggerEnter functions in order by key
	for (auto& component : name_and_component) {
		try {
			luabridge::LuaRef enabled = (*component.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.componentRef)["OnTriggerExit"](*component.second.componentRef, collision);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(actor_name, e);
		}
	}
}

void Actor::OnCollisionExit(const Collision& collision)
{
	std::vector<std::pair<std::string, Component>> name_and_component;

	// find all components on the actor that have OnCollisionExit functions
	for (auto& component : components) {
		if ((*component.second.componentRef)["OnCollisionExit"].isFunction())
			name_and_component.push_back(std::pair<std::string, Component>(component.first, component.second));
	}

	sort(name_and_component.begin(), name_and_component.end(), SortComponentsByKey());

	// call all of the OnTriggerEnter functions in order by key
	for (auto& component : name_and_component) {
		try {
			luabridge::LuaRef enabled = (*component.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.componentRef)["OnCollisionExit"](*component.second.componentRef, collision);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(actor_name, e);
		}
	}
}

void Actor::OnDestroy()
{
	std::vector<std::pair<std::string, Component>> name_and_component;

	// find all components on the actor that have OnDestroy functions
	for (auto& component_name : components_to_remove) {
		if (components.find(component_name) != components.end()) { // make sure the component exists
			if ((*components[component_name].componentRef)["OnDestroy"].isFunction())
				name_and_component.push_back(std::pair<std::string, Component>(component_name, components[component_name]));
		}
	}

	sort(name_and_component.begin(), name_and_component.end(), SortComponentsByKey());

	// call all of the OnDestroy functions in order by key
	for (auto& component : name_and_component) {
		try {
			(*component.second.componentRef)["OnDestroy"](*component.second.componentRef);
		}
		catch (const luabridge::LuaException& e) {
			ReportError(actor_name, e);
		}
	}
}
