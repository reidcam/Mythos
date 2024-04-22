#include "ComponentDBInit.h"

/// <summary>
/// initializes the component DB
/// </summary>
void ComponentDBInit::Init()
{
	InitState();
	InitFunctions();
}

void ComponentDBInit::InitFunctions() {
	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Debug")
		.addFunction("Log", &ComponentDB::CppLog)
		.addFunction("LogError", &ComponentDB::CppLogError)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginClass<Component>("Component")
		.endClass();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginClass<Actor>("Actor")
		.addFunction("GetName", &Actor::GetName)
		.addFunction("GetID", &Actor::GetID)
		.addFunction("GetComponentByKey", &Actor::GetComponentByKey)
		.addFunction("GetComponent", &Actor::GetComponent)
		.addFunction("GetComponents", &Actor::GetComponents)
		.addFunction("AddComponent", &Actor::AddComponent)
		.addFunction("RemoveComponent", &Actor::RemoveComponent)
		.endClass();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Actor")
		.addFunction("Find", &Scene::Find)
		.addFunction("FindAll", &Scene::FindAll)
		.addFunction("Instantiate", &Engine::Instantiate)
		.addFunction("Destroy", &Scene::Destroy)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Application")
		.addFunction("Quit", &ComponentDBInit::Quit)
		.addFunction("Sleep", &ComponentDBInit::Sleep)
		.addFunction("GetFrame", &ComponentDBInit::GetFrame)			
		.addFunction("OpenURL", &ComponentDBInit::OpenURL)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Input")
		.addFunction("GetKey", &Input::GetKey)
		.addFunction("GetKeyDown", &Input::GetKeyDown)
		.addFunction("GetKeyUp", &Input::GetKeyUp)
		.addFunction("GetMousePosition", &Input::GetMousePosition)
		.addFunction("GetMouseButton", &Input::GetMouseButton)
		.addFunction("GetMouseButtonDown", &Input::GetMouseButtonDown)
		.addFunction("GetMouseButtonUp", &Input::GetMouseButtonUp)
		.addFunction("GetMouseScrollDelta", &Input::GetMouseScrollDelta)
		.addFunction("GetButton", &Input::GetButton)
		.addFunction("GetButtonDown", &Input::GetButtonDown)
		.addFunction("GetButtonUp", &Input::GetButtonUp)
		.addFunction("GetAxis", &Input::GetAxis)
		.addFunction("GetControllerName", &Input::GetControllerName)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginClass<glm::vec2>("vec2")
		.addProperty("x", &glm::vec2::x)
		.addProperty("y", &glm::vec2::y)
		.endClass();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Text")
		.addFunction("Draw", &Renderer::DrawText)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Audio")
		.addFunction("Play", &AudioDB::Play)
		.addFunction("Halt", &AudioDB::Halt)
		.addFunction("SetVolume", &AudioDB::SetVolume)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Image")
		.addFunction("Draw", &Renderer::DrawImage)
		.addFunction("DrawEx", &Renderer::DrawImageEx)
		.addFunction("DrawPixel", &Renderer::DrawPixel)
		.addFunction("DrawUI", &Renderer::DrawUI)
		.addFunction("DrawUIEx", &Renderer::DrawUIEx)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Camera")
		.addFunction("SetPosition", &Renderer::SetCameraPosition)
		.addFunction("GetPositionX", &Renderer::GetCameraPositionX)
		.addFunction("GetPositionY", &Renderer::GetCameraPositionY)
		.addFunction("SetZoom", &Renderer::SetCameraZoom)
		.addFunction("GetZoom", &Renderer::GetCameraZoom)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Scene")
		.addFunction("Load", &Engine::NewSceneToLoad)
		.addFunction("GetCurrent", &Engine::GetCurrentSceneName)
		.addFunction("DontDestroy", &Scene::DontDestroyOnLoad)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginClass<b2Vec2>("Vector2")
		.addConstructor<void(*) (float, float)>()
		.addProperty("x", &b2Vec2::x)
		.addProperty("y", &b2Vec2::y)
		.addFunction("Normalize", &b2Vec2::Normalize)
		.addFunction("Length", &b2Vec2::Length)
		.addFunction("__add", &b2Vec2::operator_add)
		.addFunction("__sub", &b2Vec2::operator_sub)
		.addFunction("__mul", &b2Vec2::operator_mul)
		.endClass();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Vector2")
		.addFunction("Distance", &b2Distance)
		.addFunction("Dot", static_cast<float (*)(const b2Vec2&, const b2Vec2&)>(&b2Dot))
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginClass<Rigidbody>("Rigidbody")
		.addData("enabled", &Rigidbody::enabled)
		.addData("key", &Rigidbody::key)
		.addData("type", &Rigidbody::type)
		.addData("actor", &Rigidbody::actor)
		.addData("x", &Rigidbody::x)
		.addData("y", &Rigidbody::y)
		.addData("body_type", &Rigidbody::body_type)
		.addData("precise", &Rigidbody::precise)
		.addData("gravity_scale", &Rigidbody::gravity_scale)
		.addData("density", &Rigidbody::density)
		.addData("angular_friction", &Rigidbody::angular_friction)
		.addData("rotation", &Rigidbody::rotation)
		.addData("has_collider", &Rigidbody::has_collider)
		.addData("has_trigger", &Rigidbody::has_trigger)
		.addData("collider_type", &Rigidbody::collider_type)
		.addData("trigger_type", &Rigidbody::trigger_type)
		.addData("width", &Rigidbody::collider_width)
		.addData("height", &Rigidbody::collider_height)
		.addData("radius", &Rigidbody::collider_radius)
		.addData("trigger_width", &Rigidbody::trigger_width)
		.addData("trigger_height", &Rigidbody::trigger_height)
		.addData("trigger_radius", &Rigidbody::trigger_radius)
		.addData("friction", &Rigidbody::friction)
		.addData("bounciness", &Rigidbody::bounciness)
		.addFunction("GetPosition", &Rigidbody::GetPosition)
		.addFunction("GetRotation", &Rigidbody::GetRotation)
		.addFunction("OnStart", &Rigidbody::OnStart)
		.addFunction("AddForce", &Rigidbody::AddForce)
		.addFunction("SetVelocity", &Rigidbody::SetVelocity)
		.addFunction("SetPosition", &Rigidbody::SetPosition)
		.addFunction("SetRotation", &Rigidbody::SetRotation)
		.addFunction("SetAngularVelocity", &Rigidbody::SetAngularVelocity)
		.addFunction("SetGravityScale", &Rigidbody::SetGravityScale)
		.addFunction("SetUpDirection", &Rigidbody::SetUpDirection)
		.addFunction("SetRightDirection", &Rigidbody::SetRightDirection)
		.addFunction("GetVelocity", &Rigidbody::GetVelocity)
		.addFunction("GetAngularVelocity", &Rigidbody::GetAngularVelocity)
		.addFunction("GetGravityScale", &Rigidbody::GetGravityScale)
		.addFunction("GetUpDirection", &Rigidbody::GetUpDirection)
		.addFunction("GetRightDirection", &Rigidbody::GetRightDirection)
		.endClass();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginClass<Collision>("Collision")
		.addProperty("other", &Collision::other)
		.addProperty("point", &Collision::point)
		.addProperty("relative_velocity", &Collision::relative_velocity)
		.addProperty("normal", &Collision::normal)
		.endClass();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginNamespace("Physics")
		.addFunction("Raycast", &RaycastHandler::Raycast)
		.addFunction("RaycastAll", &RaycastHandler::RaycastAll)
		.endNamespace();

	luabridge::getGlobalNamespace(StateDB::lua_state)
		.beginClass<HitResult>("HitResult")
		.addProperty("actor", &HitResult::actor)
		.addProperty("point", &HitResult::point)
		.addProperty("normal", &HitResult::normal)
		.addProperty("is_trigger", &HitResult::is_trigger)
		.endClass();
}

/// <summary>
/// quit the application
/// </summary>
void ComponentDBInit::Quit()
{
	exit(0);
}

/// <summary>
/// the app will sleep for the specified number of seconds
/// </summary>
/// <param name="milliseconds"></param>
void ComponentDBInit::Sleep(const float milliseconds)
{
	int milliseconds_int = static_cast<int>(milliseconds);
	std::chrono::milliseconds duration(milliseconds_int);
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

/// <summary>
/// returns the current frame
/// </summary>
/// <returns></returns>
int ComponentDBInit::GetFrame()
{
	return Helper::GetFrameNumber();
}

/// <summary>
/// opens the specified url in the browser
/// </summary>
/// <param name="url"></param>
void ComponentDBInit::OpenURL(const std::string& url)
{
	#if defined(_WIN32) || defined(_WIN64)
		// Windows
		std::string command = "start " + url;
		system(command.c_str());
	#elif defined(__APPLE__) && defined(__MACH__)
		// macOS
		std::string command = "open " + url;
		system(command.c_str());
	#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
		// Linux
		std::string command = "xdg-open " + url;
		system(command.c_str());
	#else
		std::cerr << "Platform not supported!" << std::endl;
	#endif
}

/// <summary>
/// initializes Lua state
/// </summary>
void ComponentDBInit::InitState()
{
	StateDB::lua_state = luaL_newstate(); // create the new lua state
	luaL_openlibs(StateDB::lua_state); // open lua std librarys
}