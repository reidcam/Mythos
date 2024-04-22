#include "TextDB.h"

void TextDB::LoadFont(const std::string font_name, const int font_size)
{
	FontExists(font_name); // check to make sure there is a font file for the specified font
	std::string path = "resources/fonts/" + font_name + ".ttf"; // create the file path;

	// if font name hasn't been inserted, insert it
	if (loaded_fonts.find(font_name) == loaded_fonts.end()) { 
		std::unordered_map<int, TTF_Font*> font_map;
		loaded_fonts.insert({ font_name, font_map });
	}

	// this size hasn't been loaded before, load it
	if (loaded_fonts.at(font_name).find(font_size) == loaded_fonts.at(font_name).end()) { 
		loaded_fonts.at(font_name).insert({ font_size, TTF_OpenFont(path.c_str(), font_size) });
	}

}

/// <summary>
/// Converts the inputted text into an SDL_Texture*
/// </summary>
SDL_Texture* TextDB::ConvertTextToTexture(SDL_Renderer* renderer, const std::string& text, const SDL_Color& font_color, const std::string font_name, const int font_size)
{
	LoadFont(font_name, font_size);

	SDL_Surface* text_surface = TTF_RenderText_Solid(loaded_fonts.at(font_name).at(font_size), text.c_str(), font_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);

	return text_texture;
}

/// <summary>
/// returns true if the font exists and errors if it doesn't
/// </summary>
/// <param name="font_name"></param>
/// <returns></returns>
bool TextDB::FontExists(const std::string& font_name)
{
	std::string file_path = "resources/fonts/" + font_name + ".ttf";

	if (!std::filesystem::exists(file_path)) {
		std::cout << "error: font " + font_name + " missing";
		exit(0);
	}
	return true;
}