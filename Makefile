main: main.cpp src/renderer.cpp src/flappy.cpp src/pipes.cpp
	g++ src/flappy.cpp src/pipes.cpp src/renderer.cpp main.cpp -o game -I include -I dependencies/SDL2/include -L dependencies/SDL2/lib -L dependencies/SDL2_ttf/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
