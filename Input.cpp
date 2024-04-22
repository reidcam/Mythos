#include "Input.h"
const std::unordered_map<std::string, SDL_Scancode> Input::__keycode_to_scancode = {
	// Directional (arrow) Keys
	{"up", SDL_SCANCODE_UP},
	{"down", SDL_SCANCODE_DOWN},
	{"right", SDL_SCANCODE_RIGHT},
	{"left", SDL_SCANCODE_LEFT},

	// Misc Keys
	{"escape", SDL_SCANCODE_ESCAPE},

	// Modifier Keys
	{"lshift", SDL_SCANCODE_LSHIFT},
	{"rshift", SDL_SCANCODE_RSHIFT},
	{"lctrl", SDL_SCANCODE_LCTRL},
	{"rctrl", SDL_SCANCODE_RCTRL},
	{"lalt", SDL_SCANCODE_LALT},
	{"ralt", SDL_SCANCODE_RALT},

	// Editing Keys
	{"tab", SDL_SCANCODE_TAB},
	{"return", SDL_SCANCODE_RETURN},
	{"enter", SDL_SCANCODE_RETURN},
	{"backspace", SDL_SCANCODE_BACKSPACE},
	{"delete", SDL_SCANCODE_DELETE},
	{"insert", SDL_SCANCODE_INSERT},

	// Character Keys
	{"space", SDL_SCANCODE_SPACE},
	{"a", SDL_SCANCODE_A},
	{"b", SDL_SCANCODE_B},
	{"c", SDL_SCANCODE_C},
	{"d", SDL_SCANCODE_D},
	{"e", SDL_SCANCODE_E},
	{"f", SDL_SCANCODE_F},
	{"g", SDL_SCANCODE_G},
	{"h", SDL_SCANCODE_H},
	{"i", SDL_SCANCODE_I},
	{"j", SDL_SCANCODE_J},
	{"k", SDL_SCANCODE_K},
	{"l", SDL_SCANCODE_L},
	{"m", SDL_SCANCODE_M},
	{"n", SDL_SCANCODE_N},
	{"o", SDL_SCANCODE_O},
	{"p", SDL_SCANCODE_P},
	{"q", SDL_SCANCODE_Q},
	{"r", SDL_SCANCODE_R},
	{"s", SDL_SCANCODE_S},
	{"t", SDL_SCANCODE_T},
	{"u", SDL_SCANCODE_U},
	{"v", SDL_SCANCODE_V},
	{"w", SDL_SCANCODE_W},
	{"x", SDL_SCANCODE_X},
	{"y", SDL_SCANCODE_Y},
	{"z", SDL_SCANCODE_Z},
	{"0", SDL_SCANCODE_0},
	{"1", SDL_SCANCODE_1},
	{"2", SDL_SCANCODE_2},
	{"3", SDL_SCANCODE_3},
	{"4", SDL_SCANCODE_4},
	{"5", SDL_SCANCODE_5},
	{"6", SDL_SCANCODE_6},
	{"7", SDL_SCANCODE_7},
	{"8", SDL_SCANCODE_8},
	{"9", SDL_SCANCODE_9},
	{"/", SDL_SCANCODE_SLASH},
	{";", SDL_SCANCODE_SEMICOLON},
	{"=", SDL_SCANCODE_EQUALS},
	{"-", SDL_SCANCODE_MINUS},
	{".", SDL_SCANCODE_PERIOD},
	{",", SDL_SCANCODE_COMMA},
	{"[", SDL_SCANCODE_LEFTBRACKET},
	{"]", SDL_SCANCODE_RIGHTBRACKET},
	{"\\", SDL_SCANCODE_BACKSLASH},
	{"'", SDL_SCANCODE_APOSTROPHE},
	{"\"", SDL_SCANCODE_APOSTROPHE}
};

const std::unordered_map<std::string, SDL_GameControllerButton> Input::__controller_keycode_to_scancode = {
	// D Pad
	{"up", SDL_CONTROLLER_BUTTON_DPAD_UP},
	{ "down", SDL_CONTROLLER_BUTTON_DPAD_DOWN },
	{ "left", SDL_CONTROLLER_BUTTON_DPAD_LEFT },
	{ "right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT },

	// buttons
	{ "a", SDL_CONTROLLER_BUTTON_A },
	{ "b", SDL_CONTROLLER_BUTTON_B },
	{ "x", SDL_CONTROLLER_BUTTON_X },
	{ "y", SDL_CONTROLLER_BUTTON_Y },
	{ "back", SDL_CONTROLLER_BUTTON_BACK },
	{ "guide", SDL_CONTROLLER_BUTTON_GUIDE },
	{ "start", SDL_CONTROLLER_BUTTON_START },
	{ "shoulder_left", SDL_CONTROLLER_BUTTON_LEFTSHOULDER },
	{ "shoulder_right", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER },

	// stricks
	{ "stick_left", SDL_CONTROLLER_BUTTON_LEFTSTICK },
	{ "stick_right", SDL_CONTROLLER_BUTTON_RIGHTSTICK },

	// misc buttons
	{ "misc", SDL_CONTROLLER_BUTTON_MISC1 }, /* Xbox Series X share button, PS5 microphone button, Switch Pro capture button, etc. */
	{ "paddle1", SDL_CONTROLLER_BUTTON_PADDLE1 }, /* Xbox Elite paddle P1 */
	{ "paddle2", SDL_CONTROLLER_BUTTON_PADDLE2 }, /* Xbox Elite paddle P2 */
	{ "paddle3", SDL_CONTROLLER_BUTTON_PADDLE3 }, /* Xbox Elite paddle P3 */
	{ "paddle4", SDL_CONTROLLER_BUTTON_PADDLE4 }, /* Xbox Elite paddle P4 */
	{ "touchpad", SDL_CONTROLLER_BUTTON_TOUCHPAD }, /* PS4/PS5 touchpad button */
	{ "max", SDL_CONTROLLER_BUTTON_MAX }
};

/// <summary>
/// initializes the input manager
/// </summary>
void Input::Init()
{
	SDL_Init(SDL_INIT_GAMECONTROLLER); // initialize controller input

	// all inputs begin in the UP state
	for (int code = SDL_SCANCODE_UNKNOWN; code < SDL_NUM_SCANCODES; code++) {
		keyboard_states[static_cast<SDL_Scancode>(code)] = INPUT_STATE_UP;
	}

	// set all controller buttons to be up initially
	for (int button = SDL_CONTROLLER_BUTTON_A; button < SDL_CONTROLLER_BUTTON_MAX; button++) {
		controller_button_states[button] = INPUT_STATE_UP;
	}

	// set all controller joystick values to initiall be 0
	const int NUM_JOYSTICK_PARAMETERS = 6;
	for (int i = 0; i < NUM_JOYSTICK_PARAMETERS; i++) {
		joystick_axis_values.push_back(0.0f);
	}

	mouse_button_states.insert(std::pair<int, INPUT_STATE>(1, INPUT_STATE_UP)); // left mouse
	mouse_button_states.insert(std::pair<int, INPUT_STATE>(2, INPUT_STATE_UP)); // middle mouse
	mouse_button_states.insert(std::pair<int, INPUT_STATE>(3, INPUT_STATE_UP)); // right mouse
}

/// <summary>
/// processes input events
/// </summary>
/// <param name="e"></param>
void Input::ProcessEvent(const SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN) {
		if (keyboard_states.at(e.key.keysym.scancode) != INPUT_STATE_DOWN &&
			keyboard_states.at(e.key.keysym.scancode) != INPUT_STATE_JUST_BECAME_DOWN) {
			keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_DOWN;
			just_became_down_scancodes.push_back(e.key.keysym.scancode);
		}
	}
	else if (e.type == SDL_KEYUP) {
		if (keyboard_states.at(e.key.keysym.scancode) != INPUT_STATE_UP &&
			keyboard_states.at(e.key.keysym.scancode) != INPUT_STATE_JUST_BECAME_UP) {
			keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_UP;
			just_became_up_scancodes.push_back(e.key.keysym.scancode);
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_DOWN;
	}
	else if (e.type == SDL_MOUSEBUTTONUP) {
		mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_UP;
	}
	else if (e.type == SDL_CONTROLLERBUTTONDOWN) {
		controller_button_states[e.cbutton.button] = INPUT_STATE_JUST_BECAME_DOWN;
	}
	else if (e.type == SDL_CONTROLLERBUTTONUP) {
		controller_button_states[e.cbutton.button] = INPUT_STATE_JUST_BECAME_UP;
	}
	else if (e.type == SDL_CONTROLLERAXISMOTION) {
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
			joystick_axis_values[e.caxis.axis] = e.caxis.value;
		}
		else if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
			joystick_axis_values[e.caxis.axis] = e.caxis.value;
		}
		else if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
			joystick_axis_values[e.caxis.axis] = e.caxis.value;
		}
		else if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
			joystick_axis_values[e.caxis.axis] = e.caxis.value;
		}
		else if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
			joystick_axis_values[e.caxis.axis] = e.caxis.value;
		}
		else if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
			joystick_axis_values[e.caxis.axis] = e.caxis.value;
		}
	}

	// needs to be updated every frame
	if (e.type == SDL_MOUSEWHEEL && !mouse_scroll_set_this_frame) {
		mouse_scroll_this_frame = e.wheel.preciseY;
		mouse_scroll_set_this_frame = true;
	}

	if (e.type == SDL_MOUSEMOTION) {
		mouse_position = glm::vec2(e.motion.x, e.motion.y);
	}
}

void Input::LateUpdate()
{
	// keyboard updates
	for (const SDL_Scancode& code : just_became_down_scancodes) {
		keyboard_states[code] = INPUT_STATE_DOWN;
	}
	just_became_down_scancodes.clear();

	for (const SDL_Scancode& code : just_became_up_scancodes) {
		keyboard_states[code] = INPUT_STATE_UP;
	}
	just_became_up_scancodes.clear();

	// mouse updates
	for (auto& button : mouse_button_states) {
		if (button.second == INPUT_STATE_JUST_BECAME_UP)
			button.second = INPUT_STATE_UP;
		else if (button.second == INPUT_STATE_JUST_BECAME_DOWN)
			button.second = INPUT_STATE_DOWN;
	}

	// controller updates

	SetMouseScrollDeltaBool(false); // reset the mouse scroll delta bool
}

/// <summary>
/// returns whether or not the specified key is currently down
/// </summary>
/// <param name="keycode"></param>
/// <returns></returns>
bool Input::GetKey(const std::string& keycode)
{
	if (__keycode_to_scancode.find(keycode) != __keycode_to_scancode.end())
		return ((keyboard_states[__keycode_to_scancode.at(keycode)] == INPUT_STATE_DOWN) ||
			(keyboard_states[__keycode_to_scancode.at(keycode)] == INPUT_STATE_JUST_BECAME_DOWN));
	else
		return false;
}

/// <summary>
/// returns true if the key was pressed down this very frame
/// </summary>
/// <param name="keycode"></param>
/// <returns></returns>
bool Input::GetKeyDown(const std::string& keycode)
{
	if (__keycode_to_scancode.find(keycode) != __keycode_to_scancode.end())
		return keyboard_states[__keycode_to_scancode.at(keycode)] == INPUT_STATE_JUST_BECAME_DOWN;
	else
		return false;
}

/// <summary>
/// returns true if the key was released this frame
/// </summary>
/// <param name="keycode"></param>
/// <returns></returns>
bool Input::GetKeyUp(const std::string& keycode)
{
	if (__keycode_to_scancode.find(keycode) != __keycode_to_scancode.end())
		return keyboard_states[__keycode_to_scancode.at(keycode)] == INPUT_STATE_JUST_BECAME_UP;
	else
		return false;
}

glm::vec2 Input::GetMousePosition()
{
	return mouse_position;
}

bool Input::GetMouseButton(int button)
{
	if (button == 1 || button == 2 || button == 3)
		return ((mouse_button_states.at(button) == INPUT_STATE_DOWN) ||
			(mouse_button_states.at(button) == INPUT_STATE_JUST_BECAME_DOWN));
	else
		return false;
}

/// <summary>
/// returns true if the left mouse button just became down
/// </summary>
/// <returns></returns>
bool Input::GetMouseButtonDown(int button)
{
	if (button == 1 || button == 2 || button == 3)
		return mouse_button_states.at(button) == INPUT_STATE_JUST_BECAME_DOWN;
	else
		return false;
}

/// <summary>
/// returns true if the left mouse button just became up
/// </summary>
/// <returns></returns>
bool Input::GetMouseButtonUp(int button)
{
	if (button == 1 || button == 2 || button == 3)
		return mouse_button_states.at(button) == INPUT_STATE_JUST_BECAME_UP;
	else
		return false;
}

float Input::GetMouseScrollDelta()
{
	return mouse_scroll_this_frame;
}

void Input::SetMouseScrollDelta(float delta)
{
	if (!mouse_scroll_set_this_frame) {
		mouse_scroll_this_frame = delta;
		mouse_scroll_set_this_frame = true;
	}

}

void Input::SetMouseScrollDeltaBool(bool state)
{
	mouse_scroll_set_this_frame = state;
}

bool Input::GetButton(const std::string& keycode)
{
	if (__controller_keycode_to_scancode.find(keycode) != __controller_keycode_to_scancode.end())
		return ((controller_button_states[__controller_keycode_to_scancode.at(keycode)] == INPUT_STATE_DOWN) ||
			(controller_button_states[__controller_keycode_to_scancode.at(keycode)] == INPUT_STATE_JUST_BECAME_DOWN));
	else
		return false;
}

bool Input::GetButtonDown(const std::string& keycode)
{
	if (__controller_keycode_to_scancode.find(keycode) != __controller_keycode_to_scancode.end())
		return controller_button_states[__controller_keycode_to_scancode.at(keycode)] == INPUT_STATE_JUST_BECAME_DOWN;
	else
		return false;
}

bool Input::GetButtonUp(const std::string& keycode)
{
	if (__controller_keycode_to_scancode.find(keycode) != __controller_keycode_to_scancode.end())
		return controller_button_states[__controller_keycode_to_scancode.at(keycode)] == INPUT_STATE_JUST_BECAME_UP;
	else
		return false;
}

float Input::GetAxis(const std::string& axis)
{
	float return_val = 0.0f;
	if (axis == "Horizontal") { // left stick horizontal
		return_val = joystick_axis_values[0];
	}
	else if (axis == "Vertical") { // left stick vertical
		return_val = joystick_axis_values[1];
	}
	else if (axis == "RightStickHorizontal") { // right stick horizontal
		return_val = joystick_axis_values[2];
	}
	else if (axis == "RightStickVertical") { // right stick vertical
		return_val = joystick_axis_values[3];
	}
	else if (axis == "LeftTrigger") { // left trigger
		return_val = joystick_axis_values[4];
	}
	else if (axis == "RightTrigger") { // right trigger
		return_val = joystick_axis_values[5];
	}
	else {
		std::cerr << "error GetAxis no valid axis provided. Axis provided: " << axis << std::endl;
	}

	return return_val;
}

bool Input::GetShiftModState()
{
	return SDL_GetModState() & KMOD_SHIFT;
}

void Input::CheckForGamePad()
{
	const int num_joysticks = SDL_NumJoysticks();
	if (num_joysticks == 0) { // clear if no controller
		if (gameController != nullptr) { // close previous controller
			SDL_GameControllerClose(gameController);
		}
		gameController = nullptr;
	}



	// Check for game controllers
	if (gameController == nullptr) {

		for (int i = 0; i < num_joysticks; ++i) {
			if (SDL_IsGameController(i)) {
				gameController = SDL_GameControllerOpen(i);
				if (gameController) {
					break;
				}
				else {
					std::cout << "Could not open gamecontroller " << i << "! SDL Error: " << SDL_GetError() << std::endl;
				}
			}
		}
	}
}

std::string Input::GetControllerName()
{
	std::string controller_name = "no controller currently connected";
	if (gameController != nullptr) {
		controller_name = SDL_GameControllerName(gameController);
	}

	return controller_name;
}
