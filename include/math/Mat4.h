#ifndef MAGLfloat4_H
#define MAGLfloat4_H
#include <math.h>
#include "math/Quaternion.h"
#include "math/Vec3.h"

class Mat4{
private:
	GLfloat * elements;
	void setElements(GLfloat* elements);
public:
	Mat4(GLfloat value);
	GLfloat * getElements();
	~Mat4();
	static Mat4 * identityMatrix();
	static Mat4 * translationMatrix(GLfloat x, GLfloat y , GLfloat z);
	static Mat4 * rotationMatrixFromQuaternion(Quaternion* q);
	static Mat4 * scaleMatrix(GLfloat x, GLfloat y , GLfloat z);
	void scalarProduct(GLfloat scalar);
	Mat4 * copy();
	static Mat4 * rotationMatrix(GLfloat x , GLfloat y, GLfloat z);
	static Mat4 * perspectiveMatrix(GLfloat fov, GLfloat aspectRatio, GLfloat zNear, GLfloat zFar);
	static Mat4 * crossProductMatrices(Mat4 * m1, Mat4 * m2);
	void crossProduct(Mat4 * mat);
	Mat4 * getTraspose();
	static Mat4* lookAt(Vec3* eye, Vec3* target, Vec3* up);
};

#endif