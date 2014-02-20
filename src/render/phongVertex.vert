#version 410
struct Material{
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
};
in vec3 position;
in vec3 normal;
out Material objectMaterial;
out vec4 vertexNormal;
out vec4 worldSpacePosition;
layout(std140) uniform globalMatrices{
	mat4 worldMatrix;
	mat4 projectionMatrix;
};
uniform mat4 modelMatrix;
uniform Material material;
void main(){
	vec4 pos = vec4(position,1.0);
	vec4 modelSpace = modelMatrix * pos;
	vec4 worldSpace = worldMatrix * modelSpace;
	gl_Position = projectionMatrix * worldSpace;
	objectMaterial = material;
	worldSpacePosition = worldSpace;
	vertexNormal = normalize(worldMatrix * modelMatrix * vec4(normal,0.0));
}