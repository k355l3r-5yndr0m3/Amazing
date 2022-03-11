PWD=`pwd`
CFLAGS=`pkg-config --libs sdl2 SDL2_image SDL2_ttf glm assimp`
LINKERS=`pkg-config --cflags sdl2 SDL2_image SDL2_ttf glm assimp`

CPP=c++
CC=cc
EXECUTABLE=game

build: build_directory executable

executable: main.o glad.o resource_loader.o ShaderProgram.o Camera.o
	$(CPP) build/main.o build/glad.o build/resource_loader.o build/ShaderProgram.o build/Camera.o $(CFLAGS) -o $(EXECUTABLE)

clean: 
	rm $(EXECUTABLE)
	rm build/*.o

main.o: src/main.cpp
	$(CPP) -c src/main.cpp $(LINKERS) -o build/main.o

glad.o: src/glad.c
	$(CC) -c src/glad.c $(LINKERS) -o build/glad.o

resource_loader.o: src/resource_loader.cpp
	$(CPP) -c src/resource_loader.cpp $(LINKERS) -o build/resource_loader.o

ShaderProgram.o: src/ShaderProgram.cpp
	$(CPP) -c src/ShaderProgram.cpp $(LINKERS) -o build/ShaderProgram.o

Camera.o:
	$(CPP) -c src/Camera.cpp $(LINKERS) -o build/Camera.o

build_directory:
	mkdir build 2> /dev/null && echo "Creating build directory" || echo "Build directory existed. Ignore"

