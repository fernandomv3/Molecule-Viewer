#include "material/PhongMaterial.h"
#include <string.h>
#include <cassert>
#include <stdio.h>
#include "scene/Scene.h"

PhongMaterial::PhongMaterial():Material(){
	this->type = PHONG_MATERIAL;
	this->vertexShaderSource= strdup(
		"#version 440 core\n\
		#define NUM_OBJECTS %d\n\
		layout(std430) buffer modelMatrices{\n\
			mat4 modelMatrix[NUM_OBJECTS];\n\
		};\n\
		in vec3 normal;\n\
		in vec3 position;\n\
		in int drawID;\n\
		flat out int vDrawID;\n\
		out vec4 vertexNormal;\n\
		out vec4 worldSpacePosition;\n\
		layout(std140) uniform globalMatrices{\n\
			mat4 worldMatrix;\n\
			mat4 projectionMatrix;\n\
		};\n\
		void main(){\n\
			vDrawID = drawID;\n\
			vec4 pos = vec4(position,1.0);\n\
			vec4 modelSpace = modelMatrix[drawID] * pos;\n\
			vec4 worldSpace = worldMatrix * modelSpace;\n\
			gl_Position = projectionMatrix * worldSpace;\n\
			worldSpacePosition = worldSpace;\n\
			vertexNormal = normalize(worldMatrix * modelMatrix[drawID] * vec4(normal,0.0));\n\
		}");
    this->fragmentShaderSource=strdup(
    	"#version 440 core\n\
    	#define NUM_OBJECTS %d\n\
    	#define NUM_MATERIALS %d\n\
    	#define MAX_DIR_LIGHTS %d\n\
		#define MAX_P_LIGHTS %d\n\
		#if MAX_DIR_LIGHTS > 0\n\
		struct DirectionalLight{\n\
			vec4 color;\n\
			vec4 vectorToLight;\n\
			float intensity;\n\
		};\n\
		#endif\n\
		\n\
		#if MAX_P_LIGHTS > 0\n\
		struct PointLight{\n\
			vec4 color;\n\
			vec4 position;\n\
			float intensity;\n\
			float attenuation;\n\
		};\n\
		#endif\n\
		\n\
		struct Material{\n\
			vec4 diffuseColor;\n\
			vec4 specularColor;\n\
			float shininess;\n\
		};\n\
		\n\
		#if MAX_DIR_LIGHTS > 0\n\
		layout(std140) uniform directionalLights{\n\
			DirectionalLight dirLights[MAX_DIR_LIGHTS];\n\
		};\n\
		#endif\n\
		#if MAX_P_LIGHTS > 0\n\
		layout(std140) uniform pointLights{\n\
			PointLight pLights[MAX_P_LIGHTS];\n\
		};\n\
		#endif\n\
		layout(std140) uniform ambLight{\n\
			vec4 ambientLight;\n\
		};\n\
		\n\
		struct Indices{\n\
			int materialIndex;\n\
			int visible;\n\
			float distanceToCamera;\n\
		};\n\
		layout(std430) uniform materialIndices{\n\
			Indices index[NUM_OBJECTS];\n\
		};\n\
		layout(std140) uniform materials{\n\
			Material material[NUM_MATERIALS];\n\
		};\n\
    	in vec4 vertexNormal;\n\
		in vec4 worldSpacePosition;\n\
		flat in int vDrawID;\n\
    	out vec4 outputColor;\n\
    	#if MAX_P_LIGHTS > 0\n\
    	vec4 attenuateLight(in vec4 color, in float attenuation, in vec4 vectorToLight){\n\
			float distSqr = dot(vectorToLight,vectorToLight);\n\
			vec4 attenLightIntensity = color * (1/(1.0 + attenuation * sqrt(distSqr)));\n\
			return attenLightIntensity;\n\
    	}\n\
    	#endif\n\
    	\n\
    	float warp (in float value,in float factor){\n\
    		return (value + factor ) / (1+ clamp(factor,0,1));\n\
    	}\n\
    	float calculateBlinnPhongTerm(in vec4 direction,vec4 normal, in vec4 viewDirection, in float shininess, out float cosAngIncidence){\n\
    		cosAngIncidence = dot( normal , direction);\n\
    		cosAngIncidence = warp(cosAngIncidence,1);\n\
            cosAngIncidence = clamp(cosAngIncidence, 0, 1);\n\
            vec4 halfAngle = normalize(direction + viewDirection);\n\
			float blinnPhongTerm = dot(normal, halfAngle);\n\
			blinnPhongTerm = clamp(blinnPhongTerm, 0, 1);\n\
			blinnPhongTerm = cosAngIncidence != 0.0 ? blinnPhongTerm : 0.0;\n\
			blinnPhongTerm = pow(blinnPhongTerm, shininess);\n\
			return blinnPhongTerm;\n\
    	}\n\
    	\n\
    	void main(){\n\
    		vec4 viewDirection = normalize(-worldSpacePosition);\n\
			outputColor = vec4(0.0,0.0,0.0,1.0);\n\
			#if MAX_DIR_LIGHTS > 0\n\
			for(int i=0; i< MAX_DIR_LIGHTS ;i++){\n\
				vec4 normDirection = normalize(dirLights[i].vectorToLight);\n\
				vec4 normal = normalize(vertexNormal);\n\
				float cosAngIncidence;\n\
				float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material[index[vDrawID].materialIndex].shininess,cosAngIncidence);\n\
				\n\
            	outputColor = outputColor + (dirLights[i].color * material[index[vDrawID].materialIndex].diffuseColor * cosAngIncidence);\n\
            	outputColor = outputColor + (material[index[vDrawID].materialIndex].specularColor * blinnPhongTerm);\n\
			}\n\
			#endif\n\
			#if MAX_P_LIGHTS >0\n\
			for(int i=0; i< MAX_P_LIGHTS ;i++){\n\
				vec4 difference = pLights[i].position - worldSpacePosition;\n\
				vec4 normDirection = normalize(difference);\n\
				vec4 attenLightIntensity = attenuateLight(pLights[i].color,pLights[i].attenuation,difference);\n\
				vec4 normal = normalize(vertexNormal);\n\
				float cosAngIncidence;\n\
				float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material[index[vDrawID].materialIndex].shininess,cosAngIncidence);\n\
				\n\
            	outputColor = outputColor + (attenLightIntensity * material[index[vDrawID].materialIndex].diffuseColor * cosAngIncidence);\n\
            	outputColor = outputColor + (material[index[vDrawID].materialIndex].specularColor * attenLightIntensity * blinnPhongTerm);\n\
			}\n\
			#endif\n\
            outputColor = outputColor + (material[index[vDrawID].materialIndex].diffuseColor * ambientLight);\n\
    	}");
}

void PhongMaterial::makePrograms(Scene* scene){
	this->program = new GLProgram();
	char* vs = this->configureSource(this->vertexShaderSource,scene->getDirectionalLights().size(),scene->getPointLights().size(),scene->getObjects().size());
	char* fs = this->configureSource(this->fragmentShaderSource,scene->getDirectionalLights().size(),scene->getPointLights().size(),scene->getObjects().size(),scene->getMaterials().size());
	GLuint vertexShader = this->program->compileShader(GL_VERTEX_SHADER,vs);
	GLuint fragmentShader = this->program->compileShader(GL_FRAGMENT_SHADER,fs);
	delete vs;
	delete fs;
	this->program->setVertexShader(vertexShader);
	this->program->setFragmentShader(fragmentShader);
	GLuint prog = this->program->linkProgram(vertexShader,fragmentShader);
	this->program->setProgram(prog);
	this->program->setAttrPosition(glGetAttribLocation(prog, "position"));
	this->program->setAttrNormal(glGetAttribLocation(prog, "normal"));
	this->program->setAttrDrawID(glGetAttribLocation(prog, "drawID"));
	this->program->getUniforms()->unifModelMatrix =  glGetProgramResourceIndex(prog,GL_SHADER_STORAGE_BLOCK,"modelMatrices");
	glShaderStorageBlockBinding(prog, this->program->getUniforms()->unifModelMatrix,MODEL_MATRICES_UBI);
	this->program->getUniforms()->unifMaterial = glGetUniformBlockIndex(prog,"materials");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifMaterial,MATERIALS_UBI);
	this->program->getUniforms()->unifIndices = glGetUniformBlockIndex(prog,"materialIndices");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifIndices,INDICES_UBI);
	this->program->getUniforms()->unifBlockMatrices = glGetUniformBlockIndex(prog,"globalMatrices");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockMatrices,GLOBAL_MATRICES_UBI);
	this->program->getUniforms()->unifBlockDirectionalLights = glGetUniformBlockIndex(prog,"directionalLights");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockDirectionalLights,DIRLIGHTS_UBI);
	this->program->getUniforms()->unifBlockAmbientLight = glGetUniformBlockIndex(prog,"ambLight");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockAmbientLight,AMBLIGHT_UBI);
	this->program->getUniforms()->unifBlockPointLights = glGetUniformBlockIndex(prog,"pointLights");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockPointLights,PLIGHTS_UBI);
}