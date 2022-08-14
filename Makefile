INC := -I/opt/homebrew/Cellar/sdl2/2.0.22/include/SDL2 -I/opt/homebrew/Cellar/sdl_image/1.2.12_8/include/SDL
LIB := -L/opt/homebrew/Cellar/sdl2/2.0.22/lib -L/Users/davidsimonetti/Documents/Programming/Libraries/SDL2_image-2.6.1/.libs
SRC := src
BIN := bin
all: tetris 
tetris: $(SRC)/tetris.c include/tetris.h
	gcc -std=c11 -Wall -Werror -Wconversion -Wextra $(INC) $(LIB) -lSDL2-2.0.0 -lSDL2_image $(SRC)/tetris.c -o $(BIN)/tetris
	install_name_tool -change /usr/local/lib/libSDL2_image-2.0.0.dylib /Users/davidsimonetti/Documents/Programming/Libraries/SDL2_image-2.6.1/.libs/libSDL2_image-2.0.0.dylib $(BIN)/tetris
clean:
	rm bin/*