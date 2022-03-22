PWD=`pwd`
CFLAGS=`pkg-config --libs sdl2 SDL2_image SDL2_ttf glm assimp`
LINKERS=`pkg-config --cflags sdl2 SDL2_image SDL2_ttf glm assimp`

CPP=c++
CC=cc
EXECUTABLE=game

build: build_directory executable

executable: main.o glad.o resource_loader.o ShaderProgram.o Camera.o Mesh.o Application.o
	$(CPP) build/*.o $(CFLAGS) -o $(EXECUTABLE)

clean: 
	rm build/*.o
	rm $(EXECUTABLE)

main.o: src/main.cpp
	$(CPP) -c src/main.cpp $(LINKERS) -o build/main.o

glad.o: src/glad.c
	$(CC) -c src/glad.c $(LINKERS) -o build/glad.o

resource_loader.o: src/resource_loader.cpp
	$(CPP) -c src/resource_loader.cpp $(LINKERS) -o build/resource_loader.o

ShaderProgram.o: src/ShaderProgram.cpp
	$(CPP) -c src/ShaderProgram.cpp $(LINKERS) -o build/ShaderProgram.o

Camera.o: src/Camera.cpp
	$(CPP) -c src/Camera.cpp $(LINKERS) -o build/Camera.o

Mesh.o: src/Mesh.cpp
	$(CPP) -c src/Mesh.cpp $(LINKERS) -o build/Mesh.o

Application.o: src/Application.cpp
	$(CPP) -c src/Application.cpp $(LINKERS) -o build/Application.o

build_directory:
	mkdir build 2> /dev/null && echo "Creating build directory" || echo "Build directory existed. Ignore"

