#ifndef IMAGEDB_H
#define IMAGEDB_H

#include <unordered_map>
#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"
#include <filesystem>
#include <iostream>
#include "Actor.h"

class ImageDB {


public:
	inline static std::unordered_map<std::string, SDL_Texture*> actor_views;

	static bool ImageExists(const std::string& image_name);
};

#endif