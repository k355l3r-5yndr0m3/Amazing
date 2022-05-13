#version 460 core
in vec3 fragmentPosition;
in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentTexture;

uniform sampler2D defuse;

out vec4 color;

uniform vec3 eyePos;

uniform vec3 lightSource;
uniform vec3 lightColor;

// uniform sampler2D appliedTexture;

const vec3 ambientColor = vec3(0.3f, 0.3f, 0.3f);

float rnd(float i) {
	return mod(4000.*sin(23464.345*i+45.345),10.);
}


void main() {
	vec3 normal = normalize(fragmentNormal);
	vec3 lightDir = normalize(lightSource - fragmentPosition);
	vec3 viewDir = normalize(eyePos - fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 lightDis =lightSource - fragmentPosition; 
	float distanceSquare = dot(lightDis / 2.0f, lightDis / 2.0f);
	color = vec4(fragmentColor * lightColor * ((max(dot(normal, lightDir), 0.0f) + pow(max(dot(viewDir, reflectDir), 0), 2)) + ambientColor), 1.0f) * texture(defuse, fragmentTexture);

}
