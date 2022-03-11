#version 460 core
out vec4 color;

in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentTexture;

void main() {
	color = vec4(fragmentColor, 1.0f); // * texture(appliedTexture, tCoord);
}
