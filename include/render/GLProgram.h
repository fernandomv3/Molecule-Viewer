#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <GL/glew.h>

struct uniforms {
	GLuint unifModelMatrix;
	GLuint unifBlockMatrices;
	GLuint unifBlockDirectionalLights;
	GLuint unifBlockPointLights;
	GLuint unifBlockAmbientLight;
	GLuint unifDiffuseColor;
	GLuint unifSpecularColor;
	GLuint unifShininess;
};

typedef struct uniforms* Uniforms;

class GLProgram{
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
	GLuint attrPosition;
	GLuint attrNormal;
	Uniforms uniforms;
public:
	GLProgram();
	GLuint getAttrPosition();
	void setAttrPosition(GLuint attrPosition);
	GLuint getAttrNormal();
	void setAttrNormal(GLuint attrNormal);
	GLuint getVertexShader();
	GLuint getFragmentShader();
	GLuint getProgram();
	void setVertexShader(GLuint vertexShader);
	void setFragmentShader(GLuint fragmentShader);
	void setProgram(GLuint program);
	char * getSourceFromFile(char* filename);
	GLuint compileShader(GLenum type, char* source);
	GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
	void show_info_log(GLuint object,PFNGLGETSHADERIVPROC glGet__iv,PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
	Uniforms getUniforms();
	void setUniforms(Uniforms uniforms);
	~GLProgram();
};

#endif
