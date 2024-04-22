#ifndef UIRENDERREQUEST_H
#define UIRENDERREQUEST_H

#include <string>

struct UIRenderRequest {
	std::string image_name = "";
	int x = 0;
	int y = 0;
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
	int sorting_order = 0;
};

#endif