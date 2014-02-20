#ifndef EULER_H
#define EULER_H
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include "math/Mat4.h"

class Quaternion;

class Euler{
	friend class Object3D;
	friend class Quaternion;
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	char * order;
	Quaternion* quaternion;

public:
	Euler(GLfloat x, GLfloat y, GLfloat z, const char* order);
	Euler(GLfloat x, GLfloat y, GLfloat z);
	~Euler();
	char* getOrder();
	GLfloat getX();
	void setX(GLfloat x);
	GLfloat getY();
	void setY(GLfloat y);
	GLfloat getZ();
	void setZ(GLfloat z);
	void setFromQuaternion(Quaternion* q, const char* order, bool update = true);
};

#endif
