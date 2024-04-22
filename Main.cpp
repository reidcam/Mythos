# include "Engine.h"
# include "ComponentDBInit.h"
#include "box2d/box2d.h"

int main(int argc, char* argv[]) {
	Engine game;
	ComponentDBInit::Init();
	game.LoadSettings();
	game.GameLoop();

	return 0;
} // main()
