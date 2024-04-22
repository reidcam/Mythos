#include "Engine.h"


void Engine::GameLoop()
{

	// initialize
	Input::Init();
	TextDB::Init();
	AudioDB::Init();

	while (running) {
		/////////////////////////////////////////////////////////////////////////////////////
		if (new_scene_to_load) { // if there is a new scene to load, load it
			LoadScene(new_scene_name);
			new_scene_to_load = false;
		}

		// check for new controler
		Input::CheckForGamePad();

		SDL_RenderClear(renderer->GetRenderer()); // clear the renderer with the render clear color
		current_scene.CallOnStart();
		quit = renderer->GetSceneInput(); // handles input events for the scene

		current_scene.CallOnUpdate();

		Input::LateUpdate(); // update the input manager states
		current_scene.CallOnLateUpdate();

		// handle instantiated or removed actors
		current_scene.AddActorsToList();
		current_scene.AddNewComponentLifeCycleFunctions();
		current_scene.RemoveComponentLifeCycleFunctions();
		current_scene.RemoveActorsFromList();

		Renderer::RenderAndClearAllImages();
		Renderer::RenderAndClearAllUI();
		Renderer::RenderAndClearAllText();
		Renderer::RenderAndClearAllPixels();

		if (StateDB::world_initialized) // check to see if collisions need to be initialized
			InitializeCollisions();

		StateDB::AdvanceWorld();
		Helper::SDL_RenderPresent498(renderer->GetRenderer()); // render the frame

		if (quit)
			exit(0);
		//////////////////////////////////////////////////////////////////////////////////////
	}
}

/// <summary>
/// Checks for resources, loads game.config, and loads rendering.config
/// </summary>
void Engine::LoadSettings()
{
	CheckResources();

	// must be called before renderer->init because clear colors need to be read in
	if (rendering_config_exists)
		Renderer::LoadCameraSettings();

	LoadGameSettings();

	LoadScene(current_scene.current_scene_name);
}

void Engine::LoadScene(const std::string& scene_name)
{
	// keep track of the dont destroy on load actors
	std::vector<std::shared_ptr<Actor>> dont_destroy_actors;
	for (int i = 0; i < current_scene.actor_list.size(); i++) {
		if (current_scene.actor_list[i]->dontDestroyOnLoad) {
			current_scene.actor_list[i]->fromAnotherScene = true;
			dont_destroy_actors.push_back(current_scene.actor_list[i]);
		}
	}

	// clear the old scene information
	UnLoadCurrentScene();

	// add the don't destroy on load actors back to the actor list
	// and readd their Update and Late Update components
	for (int i = 0; i < dont_destroy_actors.size(); i++) {
		current_scene.actor_list.push_back(dont_destroy_actors[i]);

		for (auto& component : dont_destroy_actors[i]->components) {
			if (component.second.hasUpdate) {
				current_scene.onUpdateQueue.push_back({ dont_destroy_actors[i], { component.first, component.second } });
			}
			if (component.second.hasLateUpdate) {
				current_scene.onLateUpdateQueue.push_back({ dont_destroy_actors[i], {component.first, component.second} });
			}
		}
	}

	if (current_scene.SceneExists(scene_name)) {
		rapidjson::Document doc;
		std::string file_path = "resources/scenes/" + scene_name + ".scene";

		// read in the document
		EngineUtils::ReadJsonFile(file_path, doc);

		if (doc.HasMember("actors") && doc["actors"].IsArray()) {
			for (rapidjson::SizeType i = 0; i < doc["actors"].Size(); ++i) {

				// create and populate the new actor
				std::shared_ptr<Actor> new_actor = std::make_shared<Actor>();

				if (doc["actors"][i].IsObject()) {
					// set actor initial values
					if (doc["actors"][i].HasMember("template")) {
						// load template data if it exists
						ActorTemplate::ApplyTemplate(new_actor, doc["actors"][i]["template"].GetString());
					}

					// assign the new actor an ID and increment the global_actor_id
					new_actor->actor_id = global_actor_id++;

					if (doc["actors"][i].HasMember("name")) {
						new_actor->actor_name = doc["actors"][i]["name"].GetString();
					}
					if (doc["actors"][i].HasMember("components")) {
						const rapidjson::Value& components = doc["actors"][i]["components"];
						for (auto& component : components.GetObject()) { // loop through the components

							// component doesn't already exist from template
							if (new_actor->components[component.name.GetString()].componentRef == nullptr) {

								std::string rigidbody_string = "Rigidbody";
								if (component.value["type"].GetString() != rigidbody_string) {
									ComponentDB::ComponentExists(component.value["type"].GetString());
									ComponentDB::InitComponent(component.value["type"].GetString());

									new_actor->components[component.name.GetString()].type = component.value["type"].GetString(); // set the type
									new_actor->components[component.name.GetString()].componentRef = std::make_shared<luabridge::LuaRef>(luabridge::newTable(StateDB::lua_state));

									// make a deep copy of the component
									ComponentDB::EstablishInheritance(*new_actor->components[component.name.GetString()].componentRef,
										*ComponentDB::component_tables.at(component.value["type"].GetString()));

									// set life cylce function variables
									ComponentDB::SetComponentLifecylceVariables(new_actor->components[component.name.GetString()]);
								}
								else { // the component is a rigidbody
									new_actor->components[component.name.GetString()] = ComponentDB::CreateRigidbody();
								}

							}
							else {
								// component already exists in template, do nothing
							}

							// add other member variables
							luabridge::LuaRef table = (*new_actor->components[component.name.GetString()].componentRef);
							// add self.key to the component
							table["key"] = component.name.GetString();
							table["enabled"] = true;
							// inject self reference
							new_actor->InjectConvenienceReference(new_actor->components[component.name.GetString()].componentRef);

							for (auto& variable : component.value.GetObject()) { // loop through the variables on each component
								if (variable.value.IsString())
									table[variable.name.GetString()] = variable.value.GetString();
								else if (variable.value.IsFloat())
									table[variable.name.GetString()] = variable.value.GetFloat();
								else if (variable.value.IsInt())
									table[variable.name.GetString()] = variable.value.GetInt();
								else if (variable.value.IsBool())
									table[variable.name.GetString()] = variable.value.GetBool();
							}

							// add the component to the onStartQueue if it has a start function
							std::pair<std::string, Component> key_component = std::pair<std::string, Component>(component.name.GetString(),
								new_actor->components[component.name.GetString()]);
							std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>> id_key_component = std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>>(new_actor, key_component);

							if (new_actor->components[component.name.GetString()].hasStart) {
								current_scene.onStartQueue.push_back(id_key_component);
							}

							// add the component to onUpdateQueue if it has a update function
							if (new_actor->components[component.name.GetString()].hasUpdate) {
								current_scene.onUpdateQueue.push_back(id_key_component);
							}

							// add the component to onLateUpdateQueue if it has a late update function
							if (new_actor->components[component.name.GetString()].hasLateUpdate) {
								current_scene.onLateUpdateQueue.push_back(id_key_component);
							}
						}
					}
					else if (!doc["actors"][i].HasMember("components") && doc["actors"][i].HasMember("template")) {
						// add the component to the onStartQueue if it has a start function
						for (auto& component : new_actor->components) {
							// create pair
							std::pair<std::string, Component> key_component = std::pair<std::string, Component>(new_actor->components[component.first].type,
								new_actor->components[component.first]);
							std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>> id_key_component = std::pair<std::shared_ptr<Actor>, std::pair<std::string, Component>>(new_actor, key_component);

							if (new_actor->components[component.first].hasStart) {
								current_scene.onStartQueue.push_back(id_key_component);
							}
							if (new_actor->components[component.first].hasUpdate) {
								current_scene.onUpdateQueue.push_back(id_key_component);
							}
							if (new_actor->components[component.first].hasLateUpdate) {
								current_scene.onLateUpdateQueue.push_back(id_key_component);
							}
						}
					}
				}

				// add the actor to the actor list
				Scene::actor_list.push_back(new_actor);
			}
		}
	}
}

void Engine::UnLoadCurrentScene()
{
	current_scene.UnloadScene();
}

std::string Engine::GetCurrentSceneName()
{
	return current_scene.current_scene_name;
}

// used to check for the resources folder and other files inside of it
void Engine::CheckResources()
{
	// check for the resources folder
	if (!std::filesystem::exists("resources")) {
		std::cout << "error: resources/ missing";
		exit(0);
	}

	// check for resources/game.config
	if (!std::filesystem::exists("resources/game.config")) {
		std::cout << "error: resources/game.config missing";
		exit(0);
	}

	// check for resources/game.config
	if (!std::filesystem::exists("resources/rendering.config")) {
		rendering_config_exists = false;
	}
	else {
		rendering_config_exists = true;
	}
}

/// <summary>
/// Reads game.config to load the initial game settings
/// </summary>
void Engine::LoadGameSettings()
{
	EngineUtils util;
	rapidjson::Document doc;
	std::string file_path = "resources/game.config";

	// read in the document
	util.ReadJsonFile(file_path, doc);

	// parse the json
	if (doc.HasMember("initial_scene")) {
		current_scene.current_scene_name = doc["initial_scene"].GetString();
	}
	else {
		std::cout << "error: initial_scene unspecified";
		exit(0);
	}
	if (doc.HasMember("game_title")) {
		game_title = doc["game_title"].GetString();
	}

	// must be called after the game_title is read in bc it needs the value from it
	// but must be called before intro_image is read in bc the renderer needs to exist
	InitRenderer();
}

/// <summary>
/// intializes the renderer
/// </summary>
void Engine::InitRenderer() const
{
	// must be called before game settings get initialized so that the renderer and window exist
	renderer->Initialize(game_title);
}

Actor* Engine::Instantiate(const std::string actor_template_name) {
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	Scene::actors_to_add.push_back(actor); // add the actor to the actor_list so that it is discoverable

	ActorTemplate::ApplyTemplate(actor, actor_template_name);

	// give the actor an id
	actor->actor_id = global_actor_id++;

	// add components to just added so that their life cycle functions can be added
	for (auto& component : actor->components) {
		actor->just_added_components.insert(std::pair<std::string, Component>(component.first, component.second));
	}

	return  actor.get();
}

void Engine::NewSceneToLoad(const std::string& scene_name)
{
	new_scene_name = scene_name;
	new_scene_to_load = true;
}