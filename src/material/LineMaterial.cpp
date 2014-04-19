#include "material/LineMaterial.h"
#include <string.h>
#include <cassert>
#include <stdio.h>

LineMaterial::LineMaterial():Material(){
	this->vertexShaderSource= strdup(
		"#version 410\n\
		struct Material{\n\
			vec4 diffuseColor;\n\
			vec4 specularColor;\n\
			float shininess;\n\
		};\n\
		uniform mat4 modelMatrix;\n\
		uniform Material material;\n\
		in vec3 normal;\n\
		in vec3 position;\n\
		layout(std140) uniform globalMatrices{\n\
			mat4 worldMatrix;\n\
			mat4 projectionMatrix;\n\
		};\n\
		out vec4 color;\n\
    	\n\
		void main(){\n\
			vec4 pos = vec4(position,1.0);\n\
			vec4 modelSpace = modelMatrix * pos;\n\
			vec4 worldSpace = worldMatrix * modelSpace;\n\
			gl_Position = projectionMatrix * worldSpace;\n\
            color = material.diffuseColor;\n\
		}");
    this->fragmentShaderSource=strdup(
        "#version 410\n\
        in vec4 color;\n\
        out vec4 outputColor;\n\
        void main(){\n\
            outputColor = color;\n\
        }");
	this->program = new GLProgram();
	GLuint vertexShader = this->program->compileShader(GL_VERTEX_SHADER,this->vertexShaderSource);
	GLuint fragmentShader = this->program->compileShader(GL_FRAGMENT_SHADER,this->fragmentShaderSource);
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
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockMatrices,0);
}