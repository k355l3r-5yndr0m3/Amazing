PWD=`pwd`
CFLAGS=`pkg-config --libs sdl2 SDL2_image SDL2_ttf glm`
LINKERS=`pkg-config --cflags sdl2 SDL2_image SDL2_ttf glm`

CPP=c++
CC=cc
EXECUTABLE=game

build: executable

executable: main.o glad.o shaders.o
	$(CPP) build/main.o build/glad.o build/shaders.o $(CFLAGS) -o $(EXECUTABLE)

clean: 
	rm $(EXECUTABLE)
	rm build/*.o

main.o: src/main.cpp
	$(CPP) -c src/main.cpp $(LINKERS) -o build/main.o

glad.o: src/glad.c
	$(CC) -c src/glad.c $(LINKERS) -o build/glad.o

shaders.o: src/shaders.cpp
	$(CPP) -c src/shaders.cpp $(LINKERS) -o build/shaders.o
