#ifndef VEC3_H
#define VEC3_H 
#include <GL/glew.h>
#include <math/Mat4.h>

class Vec3{
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
public:
	Vec3();
	Vec3(GLfloat x, GLfloat y, GLfloat z);
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();
	void setComponent(int index, GLfloat value);
	GLfloat getComponent(int index);
	Vec3* applyMatrix(Mat4<GLfloat>* matrix, GLfloat w);
	void crossProduct(Vec3* vec);
	static Vec3* crossProductVectors(Vec3* v1, Vec3* v2);
	GLfloat dotProduct(Vec3* vec);
	GLfloat distance(Vec3* vec);
	void normalize();
	GLfloat length();
};

#endif