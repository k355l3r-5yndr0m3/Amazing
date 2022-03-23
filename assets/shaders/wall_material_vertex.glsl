#version 460 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in vec2 vertexTextureCoord;

out vec3 fragmentPosition;
out vec3 fragmentColor;
out vec3 fragmentNormal;
out vec2 fragmentTexture;

uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(vertexPosition, 1);
	fragmentColor = vertexColor;
 	fragmentTexture = vertexTextureCoord;
	fragmentNormal = vertexNormal;
	fragmentPosition = gl_Position.xyz;
}
