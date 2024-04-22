#ifndef TEXTDB_H
#define TEXTDB_H

#include <unordered_map>
#include <string>
#include <vector>
#include "SDL2_ttf/SDL_ttf.h"
#include <filesystem>
#include <iostream>

class TextDB
{
public:
	inline static std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> loaded_fonts;

	inline static void Init() { TTF_Init(); };

	static void LoadFont(const std::string font_name, const int font_size);

	static SDL_Texture* ConvertTextToTexture(SDL_Renderer* renderer, const std::string& text,
		const SDL_Color& font_color, const std::string font_name, const int font_size);

	static bool FontExists(const std::string& font_name);
};

#endif