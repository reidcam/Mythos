#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>
#include "Helper.h"
#include "ImageDB.h"
#include "TextDB.h"
#include "AudioDB.h"
#include "Input.h"
#include "ImageDrawRequest.h"
#include "TextRenderRequest.h"
#include "UIRenderRequest.h"
#include "PixelDrawRequest.h"
#include "EngineUtilsh.h"
#include <deque>

class Renderer {
	SDL_Window* window = nullptr;
	inline static int x_resolution = 640;
	inline static int y_resolution = 360;
	inline static int clear_color_r = 0;
	inline static int clear_color_g = 0;
	inline static int clear_color_b = 0;
	inline static float zoom_factor = 1;
public:
	inline static std::deque<ImageDrawRequest> image_draw_request_queue;
	inline static std::deque<TextRenderRequest> text_draw_request_queue;
	inline static std::deque<UIRenderRequest> ui_draw_request_queue;
	inline static std::deque<PixelDrawRequest> pixel_draw_request_queue;
	inline static SDL_Renderer* renderer = nullptr;
	inline static glm::vec2 current_cam_pos = glm::vec2(0.0f, 0.0f);

	static void RenderAndClearAllImages();

	static void RenderAndClearAllText();

	static void RenderAndClearAllUI();

	static void RenderAndClearAllPixels();

	static void DrawUI(const std::string& image_name, const float x, const float y);

	static void DrawUIEx(const std::string& image_name, const float x, const float y, const float r, const float g,
		const float b, const float a, const float sorting_order);

	static void DrawImage(const std::string& image_name, const float x, const float y);

	static void DrawImageEx(const std::string& image_name, const float x, const float y, const float rotation_degrees,
		const float scale_x, const float scale_y, const float pivot_x, const float pivot_y, const float r, const float g,
		const float b, const float a, const float sorting_order);

	static void DrawPixel(const float x, const float y, const float r, const float g, const float b, const float a);

	static void DrawText(const std::string str_content,int x,int y, std::string font_name,
		int font_size, int r, int g, int b, int a);

	static void SetCameraPosition(const float x, const float y);

	static float GetCameraPositionX();

	static float GetCameraPositionY();

	static void SetCameraZoom(const float new_zoom_factor);

	static float GetCameraZoom();

	void Initialize(const std::string& title);

	static void LoadCameraSettings();

	static void LoadView(std::string view_name);

	bool GetSceneInput();

	SDL_Renderer* GetRenderer();

	struct CompareImageRequests {
		bool operator() (const ImageDrawRequest& request1, const ImageDrawRequest& request2) {
			return request1.sorting_order < request2.sorting_order;
		}
	};

	struct CompareUIRequests {
		bool operator() (const UIRenderRequest& request1, const UIRenderRequest& request2) {
			return request1.sorting_order < request2.sorting_order;
		}
	};
};

#endif