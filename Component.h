#ifndef COMPONENT_H
#define COMPONENT_H

#include "ThirdParty/lua-5.4.6/lua.hpp"
#include "ThirdParty/LuaBridge/LuaBridge.h"
#include <memory>

class Component {
public:
	std::shared_ptr<luabridge::LuaRef> componentRef = nullptr;
	std::string type;

	bool hasStart = false;
	bool hasUpdate = false;
	bool hasLateUpdate = false;
};

#endif