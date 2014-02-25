#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include "math/Mat4.h"
#include "math/Vec3.h"
#include "object/Object3D.h"


class Camera : public Object3D{
private:
	Mat4<GLfloat> * projectionMatrix;
	Mat4<GLfloat> * worldMatrix;
	Vec3* target;
	GLuint matricesUBO;
public:
	Mat4<GLfloat>* getProjectionMatrix();
	void setProjectionMatrix(Mat4<GLfloat>* mat);
	Mat4<GLfloat>* getWorldMatrix();
	void setWorldMatrix(Mat4<GLfloat>* mat);
	void updateWorldMatrix();
	GLuint getMatricesUBO();
	void setMatricesUBO(GLuint ubo);
	GLfloat* getMatricesArray();
	Vec3* getTarget();
	void setTarget(Vec3* target);
	Camera();
	~Camera();
};

#endif