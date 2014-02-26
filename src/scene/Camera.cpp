
#include <cstdlib>
#include "scene/Camera.h"
#include "math/Mat4.h"
#include "object/Object3D.h"

Mat4* Camera::getProjectionMatrix(){
	return this->projectionMatrix;
}

void Camera::setProjectionMatrix(Mat4* mat){
	this->projectionMatrix = mat;
}

Mat4* Camera::getWorldMatrix(){
	return this->worldMatrix;
}

void Camera::setWorldMatrix(Mat4* mat){
	this->worldMatrix = mat;
}

GLuint Camera::getMatricesUBO(){
	return this->matricesUBO;
}

void Camera::setMatricesUBO(GLuint ubo){
	this->matricesUBO = ubo;
}

Camera::Camera():Object3D(){
	this->projectionMatrix = new Mat4(0);
	this->worldMatrix = new Mat4(0);
	this->matricesUBO = 0;
	this->target = NULL;
}

GLfloat* Camera::getMatricesArray(){
	GLfloat* matrices = new GLfloat[32];
	Mat4 * worldTraspose = this->worldMatrix->getTraspose();
	Mat4 * projectionTraspose = this->projectionMatrix->getTraspose();
	memcpy(matrices,worldTraspose->getElements(),sizeof(GLfloat)*16);
	memcpy(matrices+16,projectionTraspose->getElements(),sizeof(GLfloat)*16);
	delete worldTraspose;
	delete projectionTraspose;
	return matrices;
}

void Camera::updateWorldMatrix(){
	this->updateModelMatrix();
	Mat4 * translation = Mat4::translationMatrix(
		this->getPosition()->getX() * -1,
		this->getPosition()->getY() * -1,
		this->getPosition()->getZ() * -1);

	Mat4 * rot;
	if(this->target == NULL){
		Quaternion* q = this->getQuaternion()->inverse();
		rot = Mat4::rotationMatrixFromQuaternion(q);
		delete q;
	}
	else{
		rot = this->lookAt();
	}

	Mat4 * scale = Mat4::scaleMatrix(
		1 / this->getScale()->getX(),
		1 / this->getScale()->getY(),
		1 / this->getScale()->getZ());
	if(this->worldMatrix !=NULL){
		delete this->worldMatrix;
	}
	this->worldMatrix = Mat4::identityMatrix();
	this->worldMatrix->crossProduct(scale);
	this->worldMatrix->crossProduct(rot);
	this->worldMatrix->crossProduct(translation);

	
	delete scale;
	delete rot;
	delete translation;
}

Vec3* Camera::getTarget(){
	return this->target;
}

void Camera::setTarget(Vec3* target){
	this->target = target;
}

Mat4* Camera::lookAt(){
	Vec3* up = new Vec3(0.0,1.0,0.0);
	Mat4* rot = Mat4::lookAt(this->getPosition(), this->target,up);
	delete up;
	return rot;
}

Camera::~Camera(){
	if(this->projectionMatrix != NULL)
		delete this->projectionMatrix;
	if(this->worldMatrix != NULL)
		delete this->worldMatrix;
	if(this->target != NULL)
		delete this->target;
}