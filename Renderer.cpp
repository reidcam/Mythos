#include "Renderer.h"

void Renderer::RenderAndClearAllImages()
{
	std::stable_sort(image_draw_request_queue.begin(), image_draw_request_queue.end(), CompareImageRequests());
	SDL_RenderSetScale(renderer, zoom_factor, zoom_factor);

	for (auto& request : image_draw_request_queue) {
		const int pixels_per_meter = 100;
		glm::vec2 final_rendering_position = glm::vec2(request.x, request.y) - current_cam_pos;

		LoadView(request.image_name);
		SDL_Texture* tex = ImageDB::actor_views.at(request.image_name);
		SDL_Rect tex_rect;
		SDL_QueryTexture(tex, NULL, NULL, &tex_rect.w, &tex_rect.h);

		// Apply scale
		int flip_mode = SDL_FLIP_NONE;
		if (request.scale_x < 0)
			flip_mode |= SDL_FLIP_HORIZONTAL;
		if (request.scale_y < 0)
			flip_mode |= SDL_FLIP_VERTICAL;

		float x_scale = std::abs(request.scale_x);
		float y_scale = std::abs(request.scale_y);

		tex_rect.w *= x_scale;
		tex_rect.h *= y_scale;

		SDL_Point pivot_point = { static_cast<int>(request.pivot_x * tex_rect.w), static_cast<int>(request.pivot_y * tex_rect.h) };

		glm::ivec2 cam_dimensions = glm::ivec2(x_resolution, y_resolution);

		tex_rect.x = static_cast<int>(final_rendering_position.x * pixels_per_meter + cam_dimensions.x * 0.5f * (1.0f / zoom_factor) - pivot_point.x);
		tex_rect.y = static_cast<int>(final_rendering_position.y * pixels_per_meter + cam_dimensions.y * 0.5f * (1.0f / zoom_factor) - pivot_point.y);

		// Apply tint / alpha to texture
		SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
		SDL_SetTextureAlphaMod(tex, request.a);

		// Preform draw
		Helper::SDL_RenderCopyEx498(-2, "dummy", renderer, tex, NULL, &tex_rect, request.rotation_degrees,
			&pivot_point, static_cast<SDL_RendererFlip>(flip_mode));

		// Remove tint / alpha from texture
		SDL_SetTextureColorMod(tex, 255, 255, 255);
		SDL_SetTextureAlphaMod(tex, 255);
	}

	SDL_RenderSetScale(renderer, 1, 1);

	image_draw_request_queue.clear();
}

void Renderer::RenderAndClearAllText()
{
	for (auto& request : text_draw_request_queue) {
		const int pixels_per_meter = 100;
		SDL_Color font_color = { static_cast<Uint8>(request.r), static_cast<Uint8>(request.g), static_cast<Uint8>(request.b), static_cast<Uint8>(request.a) };
		SDL_Texture* text_texture = TextDB::ConvertTextToTexture(renderer, request.text, font_color, request.font, request.size);

		SDL_Rect dest_rect;
		dest_rect.x = request.x;
		dest_rect.y = request.y;
		SDL_QueryTexture(text_texture, nullptr, nullptr, &dest_rect.w, &dest_rect.h); // get w and h from the text

		double angle = 0;
		SDL_Point* center = nullptr;
		Helper::SDL_RenderCopyEx498(-2, "dummy", renderer, text_texture, nullptr, &dest_rect, angle, center, SDL_FLIP_NONE);
		SDL_DestroyTexture(text_texture);
	}

	text_draw_request_queue.clear();
}

void Renderer::DrawText(const std::string str_content, int x, int y, std::string font_name, int font_size, int r, int g, int b, int a)
{
	TextRenderRequest obj;
	obj.text = str_content;
	obj.font = font_name;
	obj.size = font_size;
	obj.x = x;
	obj.y = y;
	obj.r = r;
	obj.g = g;
	obj.b = b;
	obj.a = a;

	text_draw_request_queue.push_back(obj);
}

void Renderer::DrawUI(const std::string& image_name, const float x, const float y)
{
	UIRenderRequest obj;
	obj.image_name = image_name;
	obj.x = static_cast<int>(x);
	obj.y = static_cast<int>(y);

	ui_draw_request_queue.push_back(obj);
}

void Renderer::DrawUIEx(const std::string& image_name, const float x, const float y, const float r, const float g,
	const float b, const float a, const float sorting_order)
{
	UIRenderRequest obj;
	obj.image_name = image_name;
	obj.x = static_cast<int>(x);
	obj.y = static_cast<int>(y);
	obj.r = static_cast<int>(r);
	obj.g = static_cast<int>(g);
	obj.b = static_cast<int>(b);
	obj.a = static_cast<int>(a);
	obj.sorting_order = static_cast<int>(sorting_order);

	ui_draw_request_queue.push_back(obj);
}

void Renderer::DrawImage(const std::string& image_name, const float x, const float y)
{
	ImageDrawRequest obj;
	obj.image_name = image_name;
	obj.x = x;
	obj.y = y;

	image_draw_request_queue.push_back(obj);
}

void Renderer::DrawImageEx(const std::string& image_name, const float x, const float y, const float rotation_degrees,
	const float scale_x, const float scale_y, const float pivot_x, const float pivot_y, const float r, const float g,
	const float b, const float a, const float sorting_order)
{
	ImageDrawRequest obj;
	obj.image_name = image_name;
	obj.x = x;
	obj.y = y;
	obj.rotation_degrees = static_cast<int>(rotation_degrees);
	obj.scale_x = scale_x;
	obj.scale_y = scale_y;
	obj.pivot_x = pivot_x;
	obj.pivot_y = pivot_y;
	obj.r = static_cast<int>(r);
	obj.g = static_cast<int>(g);
	obj.b = static_cast<int>(b);
	obj.a = static_cast<int>(a);
	obj.sorting_order = static_cast<int>(sorting_order);

	image_draw_request_queue.push_back(obj);
}

void Renderer::DrawPixel(const float x, const float y, const float r, const float g, const float b, const float a)
{
	PixelDrawRequest obj;
	obj.x = static_cast<int>(x);
	obj.y = static_cast<int>(y);
	obj.r = static_cast<int>(r);
	obj.g = static_cast<int>(g);
	obj.b = static_cast<int>(b);
	obj.a = static_cast<int>(a);

	pixel_draw_request_queue.push_back(obj);
}

void Renderer::RenderAndClearAllUI()
{
	std::stable_sort(ui_draw_request_queue.begin(), ui_draw_request_queue.end(), CompareUIRequests());
	SDL_RenderSetScale(renderer, 1, 1);

	for (auto& request : ui_draw_request_queue) {
		const int pixels_per_meter = 100;
		glm::vec2 final_rendering_position = glm::vec2(request.x, request.y);

		LoadView(request.image_name);
		SDL_Texture* tex = ImageDB::actor_views.at(request.image_name);
		SDL_Rect tex_rect;
		SDL_QueryTexture(tex, NULL, NULL, &tex_rect.w, &tex_rect.h);

		glm::ivec2 cam_dimensions = glm::ivec2(x_resolution, y_resolution);

		tex_rect.x = static_cast<int>(final_rendering_position.x);
		tex_rect.y = static_cast<int>(final_rendering_position.y);

		// Apply tint / alpha to texture
		SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
		SDL_SetTextureAlphaMod(tex, request.a);

		// Preform draw
		Helper::SDL_RenderCopyEx498(-2, "dummy", renderer, tex, NULL, &tex_rect, 0,
			NULL, SDL_FLIP_NONE);

		// Remove tint / alpha from texture
		SDL_SetTextureColorMod(tex, 255, 255, 255);
		SDL_SetTextureAlphaMod(tex, 255);
	}

	ui_draw_request_queue.clear();
}

/// <summary>
/// Initializes the game window
/// </summary>
/// <param name="title"></param>
void Renderer::Initialize(const std::string& title)
{
	window = Helper::SDL_CreateWindow498(title.c_str(), 100, 100, x_resolution, y_resolution, SDL_WINDOW_SHOWN); // create the window
	renderer = Helper::SDL_CreateRenderer498(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); // set renderer properties
	SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255); // set renderer draw color
	SDL_RenderClear(renderer); // clear the renderer with the render clear color
	IMG_Init(IMG_INIT_PNG); // needed to load images to the screen
}

void Renderer::RenderAndClearAllPixels()
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // needed to ensure that alpha works

	for (auto& request : pixel_draw_request_queue) {
		SDL_SetRenderDrawColor(renderer, request.r, request.g, request.b, request.a);
		SDL_RenderDrawPoint(renderer, request.x, request.y);
	}

	pixel_draw_request_queue.clear();

	// reset renderer when finished
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255);
}

/// <summary>
/// Reads rendering.config to load the initial camera settings
/// </summary>
void Renderer::LoadCameraSettings()
{
	rapidjson::Document doc;
	std::string file_path = "resources/rendering.config";

	// read in the document
	EngineUtils::ReadJsonFile(file_path, doc);

	// parse the JSON
	if (doc.HasMember("x_resolution")) {
		x_resolution = doc["x_resolution"].GetInt();
	}
	if (doc.HasMember("y_resolution")) {
		y_resolution = doc["y_resolution"].GetInt();
	}
	if (doc.HasMember("clear_color_r")) {
		clear_color_r = doc["clear_color_r"].GetInt();
	}
	if (doc.HasMember("clear_color_g")) {
		clear_color_g = doc["clear_color_g"].GetInt();
	}
	if (doc.HasMember("clear_color_b")) {
		clear_color_b = doc["clear_color_b"].GetInt();
	}
	if (doc.HasMember("zoom_factor")) {
		zoom_factor = doc["zoom_factor"].GetFloat();
	}
}

/// <summary>
/// loads the view into actor_views if it doesn't already exist
/// </summary>
/// <param name="view_name"></param>
/// <param name="actor"></param>
void Renderer::LoadView(std::string view_name)
{
	if (ImageDB::ImageExists(view_name)) {
		std::string path = "resources/images/" + view_name + ".png";

		if (ImageDB::actor_views.find(view_name) != ImageDB::actor_views.end()) { // view is already loaded
			return;
		}
		else {
			ImageDB::actor_views.insert({ view_name, IMG_LoadTexture(renderer, path.c_str()) });
		}
	}
}

/// <summary>
/// handles input for the scene section of the game loop
/// </summary>
/// <param name="imageDB"></param>
/// <param name="textDB"></param>
/// <param name="audioDB"></param>
bool Renderer::GetSceneInput()
{
	bool quit = false;
	SDL_Event e;
	while (Helper::SDL_PollEvent498(&e)) { // empty / consume all events in the event queue
		switch (e.type) {
		case SDL_QUIT:
			quit = true;
			break;
		default:
			Input::ProcessEvent(e);
			break;
		} // event type switch statement
	}
	
	Input::SetMouseScrollDelta(0.0f); // reset mouse scroll delta if needed
	return quit;
}

void Renderer::SetCameraPosition(const float x, const float y)
{
	current_cam_pos = { x, y };
}

float Renderer::GetCameraPositionX()
{
	return current_cam_pos.x;
}

float Renderer::GetCameraPositionY()
{
	return current_cam_pos.y;
}

void Renderer::SetCameraZoom(const float new_zoom_factor)
{
	zoom_factor = new_zoom_factor;
}

/// <summary>
/// gets a pointer to the SDL_Renderer
/// </summary>
/// <returns></returns>
SDL_Renderer* Renderer::GetRenderer()
{
	return renderer;
}

float Renderer::GetCameraZoom()
{
	return zoom_factor;
}
