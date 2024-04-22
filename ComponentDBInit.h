#ifndef COMPONENTDBINIT_H
#define COMPONENTDBINIT_H

#include "Engine.h"
#include "box2d/box2d.h"
#include "Rigidbody.h"
#include "Physics.h"
#include <thread>

class ComponentDBInit
{
public:
	static void Init();
	static void InitState();
	static void InitFunctions();

	// api functions
	// Application
	static void Quit();
	static void Sleep(const float milliseconds);
	static int GetFrame();
	static void OpenURL(const std::string& url);
};

#endif