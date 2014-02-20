#include "render/GLprogram.h"
#include <cstdio>
#include <string.h>

GLProgram::GLProgram(){
	this->vertexShader =0;
    this->fragmentShader=0;
	this->program=0;
    this->uniforms = new struct uniforms;
    this->uniforms->unifModelMatrix = 0;
    this->uniforms->unifBlockMatrices =0;
    this->uniforms->unifBlockDirectionalLights=0;
    this->uniforms->unifBlockAmbientLight =0;
}

GLuint GLProgram::getVertexShader(){
	return this->vertexShader;
}

GLuint GLProgram::getFragmentShader(){
	return this->fragmentShader;
}

GLuint GLProgram::getProgram(){
	return this->program;
}

void GLProgram::setVertexShader(GLuint vertexShader){
	this->vertexShader = vertexShader;
}

void GLProgram::setFragmentShader(GLuint fragmentShader){
	this->fragmentShader = fragmentShader;
}

void GLProgram::setProgram(GLuint program){
	this->program = program;
}

char* GLProgram::getSourceFromFile(char* filename){//rewrite not using fopen()
	FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    int *length=0;
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = new char[*length+1];
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';

    return (char*)buffer;
}

GLuint GLProgram::compileShader(GLenum type, char* source){
	GLint length = strlen(source);
    GLuint shader;
    GLint shaderOk;

    if (!source)
        return 0;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderOk);
    if (!shaderOk) {
        fprintf(stderr, "Failed to compile:\n");
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint GLProgram::linkProgram(GLuint vertexShader, GLuint fragmentShader){
	GLint programOk;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &programOk);
    if (!programOk) {
        fprintf(stderr, "Failed to link shader program:\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void GLProgram::show_info_log(GLuint object,PFNGLGETSHADERIVPROC glGet__iv,PFNGLGETSHADERINFOLOGPROC glGet__InfoLog){
	GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = new char[log_length];
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    delete[] log;
}

GLuint GLProgram::getAttrPosition(){
    return this->attrPosition;
}

void GLProgram::setAttrPosition(GLuint attrPosition){
    this->attrPosition = attrPosition;
}

GLuint GLProgram::getAttrNormal(){
    return this->attrNormal;
}
void GLProgram::setAttrNormal(GLuint attrNormal){
    this->attrNormal = attrNormal;
}

Uniforms GLProgram::getUniforms(){
    return this->uniforms;
}

void GLProgram::setUniforms(Uniforms uniforms){
    this->uniforms = uniforms;
}

GLProgram::~GLProgram(){
    if(this->uniforms != NULL) delete uniforms;
}

