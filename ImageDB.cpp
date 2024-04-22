#include "ImageDB.h"

/// <summary>
/// Makes sure that the image with the specified name exists.
/// Errors if it doesn't
/// </summary>
/// <param name="image_name"></param>
/// <param name="index"></param>
/// <returns></returns>
bool ImageDB::ImageExists(const std::string& image_name)
{
	std::string file_path = "resources/images/" + image_name + ".png";

	if (!std::filesystem::exists(file_path)) {
		std::cout << "error: missing image " + image_name;
		exit(0);
	}
	return true;
}