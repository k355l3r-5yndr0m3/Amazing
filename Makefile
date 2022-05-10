PWD=`pwd`
CFLAGS=`pkg-config --libs sdl2 SDL2_image SDL2_ttf glm assimp`
LINKERS=`pkg-config --cflags sdl2 SDL2_image SDL2_ttf glm assimp`

CPP=c++
CC=cc
EXECUTABLE=game

build: build_directory executable

executable: main.o glad.o ShaderProgram.o Camera.o Mesh3D.o Application.o Enemy.o
	$(CPP) -O3 build/*.o $(CFLAGS) -o $(EXECUTABLE) -O3

clean: 
	rm build/*.o
	rm $(EXECUTABLE)

main.o: src/main.cpp
	$(CPP) -c src/main.cpp $(LINKERS) -o build/main.o -O3

glad.o: src/glad.c
	$(CC) -c src/glad.c $(LINKERS) -o build/glad.o -O3

ShaderProgram.o: src/ShaderProgram.cpp
	$(CPP) -c src/ShaderProgram.cpp $(LINKERS) -o build/ShaderProgram.o -O3

Camera.o: src/Camera.cpp
	$(CPP) -c src/Camera.cpp $(LINKERS) -o build/Camera.o -O3

Mesh3D.o: src/Mesh3D.cpp
	$(CPP) -c src/Mesh3D.cpp $(LINKERS) -o build/Mesh3D.o -O3

Application.o: src/Application.cpp
	$(CPP) -c src/Application.cpp $(LINKERS) -o build/Application.o -O3

Enemy.o: src/Enemy.cpp
	$(CPP) -c src/Enemy.cpp $(LINKERS) -o build/Enemy.o -O3

build_directory:
	mkdir build 2> /dev/null && echo "Creating build directory" || echo "Build directory existed. Ignore"

