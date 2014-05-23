#include "material/PhongMaterial.h"
#include <string.h>
#include <cassert>
#include <stdio.h>

PhongMaterial::PhongMaterial():Material(){
	this->type = PHONG_MATERIAL;
	this->vertexShaderSource= strdup(
		"#version 410\n\
		in vec3 normal;\n\
		in vec3 position;\n\
		out vec4 vertexNormal;\n\
		out vec4 worldSpacePosition;\n\
		layout(std140) uniform globalMatrices{\n\
			mat4 worldMatrix;\n\
			mat4 projectionMatrix;\n\
		};\n\
		uniform mat4 modelMatrix;\n\
		void main(){\n\
			vec4 pos = vec4(position,1.0);\n\
			vec4 modelSpace = modelMatrix * pos;\n\
			vec4 worldSpace = worldMatrix * modelSpace;\n\
			gl_Position = projectionMatrix * worldSpace;\n\
			worldSpacePosition = worldSpace;\n\
			vertexNormal = normalize(worldMatrix * modelMatrix * vec4(normal,0.0));\n\
		}");
    this->fragmentShaderSource=strdup(
    	"#version 410\n\
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
		uniform Material material;\n\
    	in vec4 vertexNormal;\n\
		in vec4 worldSpacePosition;\n\
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
				float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n\
				\n\
            	outputColor = outputColor + (dirLights[i].color * material.diffuseColor * cosAngIncidence);\n\
            	outputColor = outputColor + (material.specularColor * blinnPhongTerm);\n\
			}\n\
			#endif\n\
			#if MAX_P_LIGHTS >0\n\
			for(int i=0; i< MAX_P_LIGHTS ;i++){\n\
				vec4 difference = pLights[i].position - worldSpacePosition;\n\
				vec4 normDirection = normalize(difference);\n\
				vec4 attenLightIntensity = attenuateLight(pLights[i].color,pLights[i].attenuation,difference);\n\
				vec4 normal = normalize(vertexNormal);\n\
				float cosAngIncidence;\n\
				float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n\
				\n\
            	outputColor = outputColor + (attenLightIntensity * material.diffuseColor * cosAngIncidence);\n\
            	outputColor = outputColor + (material.specularColor * attenLightIntensity * blinnPhongTerm);\n\
			}\n\
			#endif\n\
            outputColor = outputColor + (material.diffuseColor * ambientLight);\n\
    	}");
}

void PhongMaterial::makePrograms(int numDirLights,int numPointLights){
	this->program = new GLProgram();
	char* vs = this->configureSource(this->vertexShaderSource,numDirLights,numPointLights);
	char* fs = this->configureSource(this->fragmentShaderSource,numDirLights,numPointLights);
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
	this->program->getUniforms()->unifModelMatrix = glGetUniformLocation(prog,"modelMatrix");
	this->program->getUniforms()->unifDiffuseColor = glGetUniformLocation(prog,"material.diffuseColor");
	this->program->getUniforms()->unifSpecularColor = glGetUniformLocation(prog,"material.specularColor");
	this->program->getUniforms()->unifShininess = glGetUniformLocation(prog,"material.shininess");
	this->program->getUniforms()->unifBlockMatrices = glGetUniformBlockIndex(prog,"globalMatrices");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockMatrices,GLOBAL_MATRICES_UBI);
	this->program->getUniforms()->unifBlockDirectionalLights = glGetUniformBlockIndex(prog,"directionalLights");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockDirectionalLights,DIRLIGHTS_UBI);
	this->program->getUniforms()->unifBlockAmbientLight = glGetUniformBlockIndex(prog,"ambLight");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockAmbientLight,AMBLIGHT_UBI);
	this->program->getUniforms()->unifBlockPointLights = glGetUniformBlockIndex(prog,"pointLights");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockPointLights,PLIGHTS_UBI);
}