#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include "math/Mat4.h"
#include "math/Vec3.h"
#include "object/Object3D.h"


class Camera : public Object3D{
private:
	Mat4 * projectionMatrix;
	Mat4 * worldMatrix;
	Vec3* target;
	GLuint matricesUBO;
public:
	Mat4* getProjectionMatrix();
	void setProjectionMatrix(Mat4* mat);
	Mat4* getWorldMatrix();
	void setWorldMatrix(Mat4* mat);
	void updateWorldMatrix();
	GLuint getMatricesUBO();
	void setMatricesUBO(GLuint ubo);
	GLfloat* getMatricesArray();
	Vec3* getTarget();
	void setTarget(Vec3* target);
	Mat4* lookAt();
	Camera();
	~Camera();
};

#endif