#version 460 core
in vec3 fragmentPosition;
in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentTexture;



out vec4 color;

uniform vec3 eyePos;

uniform vec3 lightSource;
uniform vec3 lightColor;

// uniform sampler2D appliedTexture;



void main() {
	vec3 normal = normalize(fragmentNormal);
	vec3 lightDir = normalize(lightSource - fragmentPosition);
	vec3 viewDir = normalize(eyePos - fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	color = vec4(fragmentColor * (lightColor * (max(dot(normal, lightDir), 0.0f) + pow(max(dot(viewDir, reflectDir), 0), 2)) + vec3(0.02f, 0.08f, 0.1f)), 1.0f);

}
