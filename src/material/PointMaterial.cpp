#include "material/PointMaterial.h"
#include <string.h>

PointMaterial::PointMaterial():Material(){
	this->vertexShaderSource= strdup(
		"#version 410\n\
		in vec3 position;\n\
		uniform vec4 diffuseColor;\n\
		flat out vec4 color;\n\
		layout(std140) uniform globalMatrices{\n\
			mat4 worldMatrix;\n\
			mat4 projectionMatrix;\n\
		};\n\
		void main(){\n\
			vec4 pos = vec4(position,1.0);\n\
			vec4 worldPos = worldMatrix *  pos;\n\
			gl_Position = projectionMatrix * worldPos;\n\
			gl_PointSize = 10;\n\
			color = worldPos;\n\
		}");
    this->fragmentShaderSource=strdup(
    	"#version 410\n\
		uniform vec4 diffuseColor;\n\
		flat in vec4 color;\n\
    	out vec4 outputColor;\n\
    	void main(){\n\
    		outputColor = (color + 15.0) / 21.0;\n\
    	}");
	this->program = new GLProgram();
	GLuint vertexShader = this->program->compileShader(GL_VERTEX_SHADER,this->vertexShaderSource);
	GLuint fragmentShader = this->program->compileShader(GL_FRAGMENT_SHADER,this->fragmentShaderSource);
	this->program->setVertexShader(vertexShader);
	this->program->setFragmentShader(fragmentShader);
	GLuint prog = this->program->linkProgram(vertexShader,fragmentShader);
	this->program->setProgram(prog);
	this->program->setAttrPosition(glGetAttribLocation(prog, "position"));
	this->program->getUniforms()->unifDiffuseColor = glGetUniformLocation(prog,"diffuseColor");
	this->program->getUniforms()->unifBlockMatrices = glGetUniformBlockIndex(prog,"globalMatrices");
	glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockMatrices,0);
}