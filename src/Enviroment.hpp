#pragma once 

#include "glad/glad.h"

class Renderable {
	public: 
		Renderable();
		~Renderable();

		virtual void draw();
};
