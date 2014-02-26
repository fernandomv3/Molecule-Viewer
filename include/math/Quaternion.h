#ifndef QUATERNION_H
#define QUATERNION_H
#include <stdlib.h>
#include <GL/glew.h>

class Euler;
class Mat4;
class Vec3;

class Quaternion{
	friend class Object3D;
	friend class Euler;
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
	Euler* euler;
public:
	Quaternion();
	Quaternion(GLfloat x, GLfloat y, GLfloat z,GLfloat w);
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	void setW(GLfloat w);
	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();
	GLfloat getW();
	void setComponent(int index, GLfloat value);
	GLfloat getComponent(int index);
	void setFromEuler(Euler* euler , bool update = true);
	void setFromMat4(Mat4* mat4);
	Quaternion * conjugate();
	Quaternion * inverse();
	void normalize();
	GLfloat length();
	static Quaternion* multiplyQuaternions(Quaternion* q1, Quaternion* q2);
	void  multiply(Quaternion* q);
	GLfloat dotProduct(Quaternion* q);
	static GLfloat dotProductQuaternion(Quaternion* q1, Quaternion* q2);
	static Quaternion* rotationBetweenVectors(Vec3* vec1, Vec3* vec2);
	void setEuler(Euler* euler);
};

#endif