#ifndef COLOR_H
#define COLOR_H

#include <GL/glew.h>

class Color{
private:
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
public:
	Color();
	Color(GLfloat r, GLfloat g, GLfloat b);
	void setRGB255(GLint r, GLint g, GLint b);
	void setRGB(GLfloat r, GLfloat g, GLfloat b);
	void setComponent(char component, GLfloat value);
	void addColor(Color * color);
	GLfloat* getAsArray();
	GLfloat getComponent(char component);
};

#endif