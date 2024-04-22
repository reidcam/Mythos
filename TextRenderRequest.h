#ifndef TEXTRENDERREQUEST_H
#define TEXTRENDERREQUEST_H

#include <string>

struct TextRenderRequest {
	std::string text = "";
	std::string font = "";
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;
	int size = 0;
	int x = 0;
	int y = 0;
};
#endif