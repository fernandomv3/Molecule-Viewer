#version 410
#define MAX_DIR_LIGHTS 10
#define MAX_P_LIGHTS 10
struct DirectionalLight{
	vec4 color;
	vec4 vectorToLight;
	float intensity;
};

struct PointLight{
	vec4 color;
	vec4 position;
	float intensity;
	float attenuation;
};

struct Material{
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
};

layout(std140) uniform directionalLights{
	DirectionalLight dirLights[MAX_DIR_LIGHTS];
	int numDirLights;
};
layout(std140) uniform pointLights{
	PointLight pLights[MAX_P_LIGHTS];
	int numPointLights;
};
layout(std140) uniform ambLight{
	vec4 ambientLight;
};
layout(std140) uniform globalMatrices{
	mat4 worldMatrix;
	mat4 projectionMatrix;
};

in vec4 vertexNormal;
in vec4 worldSpacePosition;
in Material objectMaterial;
out vec4 outputColor;
void main(){
	vec4 viewDirection = normalize(worldSpacePosition);
	outputColor = vec4(0.0,0.0,0.0,1.0);
	for(int i=0; i< numDirLights ;i++){
		vec4 normDirection = normalize(worldMatrix * dirLights[i].vectorToLight);
		vec4 normal = normalize(vertexNormal);
		float cosAngIncidence = dot( normal , normDirection);
		cosAngIncidence = clamp(cosAngIncidence, 0, 1);
		
		vec4 halfAngle = normalize(normDirection + viewDirection);
		float blinnPhongTerm = dot(normal, halfAngle);
		blinnPhongTerm = clamp(blinnPhongTerm, 0, 1);
		blinnPhongTerm = cosAngIncidence != 0.0 ? blinnPhongTerm : 0.0;
		blinnPhongTerm = pow(blinnPhongTerm, objectMaterial.shininess);
		
    	outputColor = outputColor + (dirLights[i].color * objectMaterial.diffuseColor * cosAngIncidence) /*+ (objectMaterial.specularColor * blinnPhongTerm)*/;
	}
	for(int i=0; i< numPointLights ;i++){
		vec4 difference = pLights[i].position - worldSpacePosition;
		vec4 normDirection = normalize(difference);
		float distSqr = dot(difference,difference);
		vec4 attenLightIntensity = pLights[i].color * (1/(1.0 + pLights[i].attenuation * sqrt(distSqr)));
		vec4 normal = normalize(vertexNormal);
		float cosAngIncidence = dot( normDirection,normal);
    	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
		
		vec4 halfAngle = normalize(normDirection + viewDirection);
		float blinnPhongTerm = dot(normal, halfAngle);
		blinnPhongTerm = clamp(blinnPhongTerm, 0, 1);
		blinnPhongTerm = cosAngIncidence != 0.0 ? blinnPhongTerm : 0.0;
		blinnPhongTerm = pow(blinnPhongTerm, objectMaterial.shininess);
		
    	outputColor = outputColor + (/*attenLightIntensity */ objectMaterial.diffuseColor * cosAngIncidence)+(objectMaterial.specularColor * attenLightIntensity * blinnPhongTerm);
	}
    outputColor = outputColor + (objectMaterial.diffuseColor * ambientLight);
}