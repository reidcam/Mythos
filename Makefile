main:
	clang++ -std=c++17 -I./ -I./ThirdParty/ -I./ThirdParty/glm/ -I./ThirdParty/rapidjson-1.1.0/include/ \
	-I./ThirdParty/SDL2/ \
	-I./ThirdParty/SDL2_image/ \
	-I./ThirdParty/SDL2_mixer/ \
	-I./ThirdParty/SDL2_ttf/ \
	-I./ThirdParty/box2d-2.4.1/ \
	-I./ThirdParty/box2d-2.4.1/box2d/ \
	-I./ThirdParty/box2d-2.4.1/src/ \
	-I./ThirdParty/box2d-2.4.1/src/dynamics/ \
	-I./ThirdParty/box2d-2.4.1/src/collision/ \
	-I./ThirdParty/box2d-2.4.1/src/common/ \
	-I./ThirdParty/box2d-2.4.1/src/rope/ \
	-I./ThirdParty/lua-5.4.6 \
	-I./ThirdParty/LuaBridge \
	-I./ThirdParty/LuaBridge/detail \
	-L./ -L./ThirdParty/SDL2/lib/ \
	-L./ThirdParty/SDL2_image/lib/ \
	-L./ThirdParty/SDL2_mixer/lib/ \
	-L./ThirdParty/SDL2_ttf/lib/ \
	-L./ThirdParty/box2d-2.4.1/ \
	-L./ThirdParty/box2d-2.4.1/src/collision \
	-L./ThirdParty/box2d-2.4.1/src/common \
	-L./ThirdParty/box2d-2.4.1/src/dynamics \
	-L./ThirdParty/box2d-2.4.1/src/rope \
	-L./ThirdParty/lua-5.4.6 \
	-L./ThirdParty/LuaBridge \
	-L./ThirdParty/LuaBridge/detail \
	-O3 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua5.4 -o game_engine_linux ./*.cpp \
	./ThirdParty/box2d-2.4.1/src/dynamics/*.cpp \
	./ThirdParty/box2d-2.4.1/src/collision/*.cpp \
	./ThirdParty/box2d-2.4.1/src/common/*.cpp \
	./ThirdParty/box2d-2.4.1/src/rope/*.cpp