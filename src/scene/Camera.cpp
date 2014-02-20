
#include <cstdlib>
#include "scene/Camera.h"
#include "math/Mat4.h"
#include "object/Object3D.h"

Mat4<GLfloat>* Camera::getProjectionMatrix(){
	return this->projectionMatrix;
}

void Camera::setProjectionMatrix(Mat4<GLfloat>* mat){
	this->projectionMatrix = mat;
}

Mat4<GLfloat>* Camera::getWorldMatrix(){
	return this->worldMatrix;
}

void Camera::setWorldMatrix(Mat4<GLfloat>* mat){
	this->worldMatrix = mat;
}

GLuint Camera::getMatricesUBO(){
	return this->matricesUBO;
}

void Camera::setMatricesUBO(GLuint ubo){
	this->matricesUBO = ubo;
}

Camera::Camera():Object3D(){
	this->projectionMatrix = new Mat4<GLfloat>(0);
	this->worldMatrix = new Mat4<GLfloat>(0);
	this->matricesUBO = 0;
}

GLfloat* Camera::getMatricesArray(){
	GLfloat* matrices = new GLfloat[32];
	Mat4<GLfloat> * worldTraspose = this->worldMatrix->getTraspose();
	Mat4<GLfloat> * projectionTraspose = this->projectionMatrix->getTraspose();
	memcpy(matrices,worldTraspose->getElements(),sizeof(GLfloat)*16);
	memcpy(matrices+16,projectionTraspose->getElements(),sizeof(GLfloat)*16);
	delete worldTraspose;
	delete projectionTraspose;
	return matrices;
}

void Camera::updateWorldMatrix(){
	this->updateModelMatrix();
	Mat4<GLfloat> * translation = Mat4<GLfloat>::translationMatrix(
		this->getPosition()->getX() * -1,
		this->getPosition()->getY() * -1,
		this->getPosition()->getZ() * -1);
	Quaternion* q = this->getQuaternion()->inverse();
	Mat4<GLfloat> * rot = Mat4<GLfloat>::rotationMatrixFromQuaternion(q);

	Mat4<GLfloat> * scale = Mat4<GLfloat>::scaleMatrix(
		1 / this->getScale()->getX(),
		1 / this->getScale()->getY(),
		1 / this->getScale()->getZ());
	if(this->worldMatrix !=NULL){
		delete this->worldMatrix;
	}
	this->worldMatrix = Mat4<GLfloat>::identityMatrix();
	this->worldMatrix->crossProduct(scale);
	this->worldMatrix->crossProduct(rot);
	this->worldMatrix->crossProduct(translation);

	delete q;
	delete scale;
	delete rot;
	delete translation;
}

Camera::~Camera(){
	if(this->projectionMatrix != NULL)
		delete this->projectionMatrix;
	if(this->worldMatrix != NULL)
		delete this->worldMatrix;
}