#ifndef ENGINE_H
#define ENGINE_H

#include <cstdlib>
#include "Rigidbody.h"
#include "SceneDB.h"
#include "Renderer.h"
#include "ImageDB.h"
#include "TextDB.h"
#include "AudioDB.h"
#include "Input.h"
#include "ThirdParty/lua-5.4.6/lua.hpp"
#include "ThirdParty/LuaBridge/LuaBridge.h"
#include "CollisionDetector.h"

class Engine {
public:
	void GameLoop();
	void LoadSettings();

	static Actor* Instantiate(const std::string actor_template_name);
	static void NewSceneToLoad(const std::string& scene_name);
	static void LoadScene(const std::string& scene_name);
	static void UnLoadCurrentScene();
	static std::string GetCurrentSceneName();
	inline static Scene current_scene;
	inline static int global_actor_id = 0;
	inline static bool new_scene_to_load = false;
	inline static std::string new_scene_name = "";

	inline static CollisionDetector* collision_detector;
private:

	void CheckResources();
	void LoadGameSettings();
	void InitRenderer() const;
	int x_resolution = 640;
	int y_resolution = 360;
	float zoom_factor = 1.0f;
	bool running = true;
	bool rendering_config_exists = false;

	inline static Renderer* renderer = new Renderer();
	std::string game_title = "";
	int clear_color_r = 255;
	int clear_color_g = 255;
	int clear_color_b = 255;

	// quit var for when the player presses quit
	bool quit = false;


	inline static void InitializeCollisions() {
		collision_detector = new CollisionDetector();
		StateDB::world->SetContactListener(collision_detector);
	};
};

#endif