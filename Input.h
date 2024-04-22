#ifndef INPUT_H
#define INPUT_H

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include "Helper.h"

enum INPUT_STATE { INPUT_STATE_UP, INPUT_STATE_JUST_BECAME_DOWN, INPUT_STATE_DOWN, INPUT_STATE_JUST_BECAME_UP };

class Input
{
public:
	static void Init(); // Call before main loop begins.
	static void ProcessEvent(const SDL_Event& e); // Call every frame at start of event loop.
	static void LateUpdate(); // call at very end of frame

	static bool GetKey(const std::string& keycode);
	static bool GetKeyDown(const std::string& keycode);
	static bool GetKeyUp(const std::string& keycode);

	static glm::vec2 GetMousePosition();

	static bool GetMouseButton(int button);
	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtonUp(int button);
	static float GetMouseScrollDelta();
	static void SetMouseScrollDelta(float delta);
	static void SetMouseScrollDeltaBool(bool state);

	// methods for controllers
	static bool GetButton(const std::string& keycode);
	static bool GetButtonDown(const std::string& keycode);
	static bool GetButtonUp(const std::string& keycode);
	static float GetAxis(const std::string& axis);

	static bool GetShiftModState();

	static void CheckForGamePad();
	static std::string GetControllerName();

	static const std::unordered_map<std::string, SDL_Scancode> __keycode_to_scancode;
	static const std::unordered_map<std::string, SDL_GameControllerButton> __controller_keycode_to_scancode;
private:
	static inline SDL_GameController* gameController = nullptr;
	static inline std::unordered_map<int, INPUT_STATE> controller_button_states;
	static inline std::vector<float> joystick_axis_values;

	static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyboard_states;
	static inline std::vector<SDL_Scancode> just_became_down_scancodes;
	static inline std::vector<SDL_Scancode> just_became_up_scancodes;

	static inline glm::vec2 mouse_position;
	static inline std::unordered_map<int, INPUT_STATE> mouse_button_states;
	static inline std::vector<int> just_became_down_buttons;
	static inline std::vector<int> just_became_up_buttons;

	static inline float mouse_scroll_this_frame = 0;
	static inline bool mouse_scroll_set_this_frame = false;
};

#endif