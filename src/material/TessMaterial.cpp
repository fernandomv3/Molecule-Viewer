#include "material/TessMaterial.h"
#include <string.h>
#include <cassert>
#include <stdio.h>

TessMaterial::TessMaterial():Material(){
	this->vertexShaderSource= strdup(
		"#version 410\n\
		in vec3 normal;\n\
		in vec3 position;\n\
		out vec4 vNormal;\n\
		out vec4 vPos;\n\
		layout(std140) uniform globalMatrices{\n\
			mat4 worldMatrix;\n\
			mat4 projectionMatrix;\n\
		};\n\
		uniform mat4 modelMatrix;\n\
		void main(){\n\
			vec4 pos = vec4(position,1.0);\n\
			vec4 modelSpace = modelMatrix * pos;\n\
			vec4 worldSpace = worldMatrix * modelSpace;\n\
			vPos = worldSpace;\n\
			vNormal = normalize(worldMatrix * modelMatrix * vec4(normal,0.0));\n\
		}");
	this->tessControlShaderSource = strdup(
		"#version 410\n\
		layout(vertices = 3) out;\n\
		in vec4 vNormal[];\n\
		in vec4 vPos[];\n\
		out vec4 tcPos[];\n\
		out vec4 tcNormal[];\n\
		\n\
		#define ID gl_InvocationID\n\
		\n\
		void main(){\n\
			tcPos[ID] = vPos[ID];\n\
			tcNormal[ID] = vNormal[ID];\n\
			if(ID ==0){\n\
				gl_TessLevelInner[0] = 1;\n\
				gl_TessLevelOuter[0] = 1;\n\
				gl_TessLevelOuter[1] = 1;\n\
				gl_TessLevelOuter[2] = 1;\n\
			}\n\
		}\n\
		");
	this->tessEvaluationShaderSource= strdup(
		"#version 410\n\
		layout(triangles, equal_spacing, ccw) in;\n\
		in vec4 tcPos[];\n\
		in vec4 tcNormal[];\n\
		out vec4 tePos;\n\
		out vec4 teNormal;\n\
		layout(std140) uniform globalMatrices{\n\
			mat4 worldMatrix;\n\
			mat4 projectionMatrix;\n\
		};\n\
		void main(){\n\
			vec4 p0 = gl_TessCoord.x * tcPos[0];\n\
			vec4 p1 = gl_TessCoord.y * tcPos[1];\n\
			vec4 p2 = gl_TessCoord.z * tcPos[2];\n\
			\n\
			vec4 n0 = gl_TessCoord.x * tcNormal[0];\n\
			vec4 n1 = gl_TessCoord.y * tcNormal[1];\n\
			vec4 n2 = gl_TessCoord.z * tcNormal[2];\n\
			tePos = normalize(p0+p1+p2);\n\
			teNormal = normalize(n0+n1+n2);\n\
			gl_Position = projectionMatrix * tePos;\n\
		}");
    this->fragmentShaderSource=strdup(
    	"#version 410\n\
    	#define MAX_DIR_LIGHTS 10\n\
		#define MAX_P_LIGHTS 10\n\
		struct DirectionalLight{\n\
			vec4 color;\n\
			vec4 vectorToLight;\n\
			float intensity;\n\
		};\n\
		\n\
		struct PointLight{\n\
			vec4 color;\n\
			vec4 position;\n\
			float intensity;\n\
			float attenuation;\n\
		};\n\
		\n\
		struct Material{\n\
			vec4 diffuseColor;\n\
			vec4 specularColor;\n\
			float shininess;\n\
		};\n\
		\n\
		layout(std140) uniform directionalLights{\n\
			DirectionalLight dirLights[MAX_DIR_LIGHTS];\n\
			int numDirLights;\n\
		};\n\
		layout(std140) uniform pointLights{\n\
			PointLight pLights[MAX_P_LIGHTS];\n\
			int numPointLights;\n\
		};\n\
		layout(std140) uniform ambLight{\n\
			vec4 ambientLight;\n\
		};\n\
		\n\
		uniform Material material;\n\
    	in vec4 teNormal;\n\
		in vec4 tePos;\n\
    	out vec4 outputColor;\n\
    	vec4 attenuateLight(in vec4 color, in float attenuation, in vec4 vectorToLight){\n\
			float distSqr = dot(vectorToLight,vectorToLight);\n\
			vec4 attenLightIntensity = color * (1/(1.0 + attenuation * sqrt(distSqr)));\n\
			return attenLightIntensity;\n\
    	}\n\
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
    		vec4 viewDirection = normalize(-tePos);\n\
			outputColor = vec4(0.0,0.0,0.0,1.0);\n\
			for(int i=0; i< numDirLights ;i++){\n\
				vec4 normDirection = normalize(dirLights[i].vectorToLight);\n\
				vec4 normal = normalize(teNormal);\n\
				float cosAngIncidence;\n\
				float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n\
				\n\
            	outputColor = outputColor + (dirLights[i].color * material.diffuseColor * cosAngIncidence);\n\
            	outputColor = outputColor + (material.specularColor * blinnPhongTerm);\n\
			}\n\
			for(int i=0; i< numPointLights ;i++){\n\
				vec4 difference = pLights[i].position - tePos;\n\
				vec4 normDirection = normalize(difference);\n\
				vec4 attenLightIntensity = attenuateLight(pLights[i].color,pLights[i].attenuation,difference);\n\
				vec4 normal = normalize(teNormal);\n\
				float cosAngIncidence;\n\
				float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n\
				\n\
            	outputColor = outputColor + (attenLightIntensity * material.diffuseColor * cosAngIncidence);\n\
            	outputColor = outputColor + (material.specularColor * attenLightIntensity * blinnPhongTerm);\n\
			}\n\
            outputColor = outputColor + (material.diffuseColor * ambientLight);\n\
    	}");
	this->program = new GLProgram();
	GLuint vertexShader = this->program->compileShader(GL_VERTEX_SHADER,this->vertexShaderSource);
	GLuint fragmentShader = this->program->compileShader(GL_FRAGMENT_SHADER,this->fragmentShaderSource);
	GLuint tessControlShader = this->program->compileShader(GL_TESS_CONTROL_SHADER,this->tessControlShaderSource);
	GLuint tessEvaluationShader = this->program->compileShader(GL_TESS_EVALUATION_SHADER,this->tessEvaluationShaderSource);
	this->program->setVertexShader(vertexShader);
	this->program->setFragmentShader(fragmentShader);
	this->program->setTessControlShader(tessControlShader);
	this->program->setTessEvaluationShader(tessEvaluationShader);
	GLuint prog = this->program->linkProgramTessellation(vertexShader,fragmentShader,tessControlShader,tessEvaluationShader);
	this->program->setProgram(prog);
	this->program->setAttrPosition(glGetAttribLocation(prog, "position"));
	this->program->setAttrNormal(glGetAttribLocation(prog, "normal"));
	this->program->getUniforms()->unifModelMatrix = glGetUniformLocation(prog,"modelMatrix");
	this->program->getUniforms()->unifDiffuseColor = glGetUniformLocation(prog,"material.diffuseColor");
	this->program->getUniforms()->unifSpecularColor = glGetUniformLocation(prog,"material.specularColor");
	this->program->getUniforms()->unifShininess = glGetUniformLocation(prog,"material.shininess");
	this->program->getUniforms()->unifBlockMatrices = glGetUniformBlockIndex(prog,"globalMatrices");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockMatrices,0);
	this->program->getUniforms()->unifBlockDirectionalLights = glGetUniformBlockIndex(prog,"directionalLights");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockDirectionalLights,1);
	this->program->getUniforms()->unifBlockAmbientLight = glGetUniformBlockIndex(prog,"ambLight");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockAmbientLight,2);
	this->program->getUniforms()->unifBlockPointLights = glGetUniformBlockIndex(prog,"pointLights");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockPointLights,3);
}