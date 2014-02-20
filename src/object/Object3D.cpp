#include "object/Object3D.h"
#include <cstdlib>

Object3D::Object3D(){
	this->position = new Vec3(0,0,0);
	this->rotation = new Euler(0,0,0,"XYZ");
	this->quaternion = new Quaternion(0,0,0,1);
	this->scale = new Vec3(1,1,1);
	this->modelMatrix = Mat4<GLfloat>::identityMatrix();

	this->rotation->quaternion = this->quaternion;
	this->quaternion->euler=this->rotation;
	//this->quaternion->setFromEuler(this->rotation,false);
}

Object3D::~Object3D(){
	if (this->position != NULL){
		delete this->position;
	}
	if (this->rotation != NULL){
		delete this->rotation;
	}
	if(this->scale != NULL){
		delete this->scale;
	}
	if(this->quaternion != NULL){
		delete this->quaternion;
	}
	if(this->modelMatrix != NULL)
		delete this->modelMatrix;
}

Vec3* Object3D::getPosition(){
	return this->position;
}

Euler* Object3D::getRotation(){
	return this->rotation;
}

Vec3* Object3D::getScale(){
	return this->scale;
}

Quaternion* Object3D::getQuaternion(){
	return this->quaternion;
}

void Object3D::setPosition(Vec3* position){
	this->position = position;
}

void Object3D::setRotation(Euler* rotation){
	this->rotation = rotation;
	//delete previous quaternion in new euler if exists
	if (rotation->quaternion != NULL)
		delete[] (rotation->quaternion);
	rotation->quaternion = this->quaternion;

	//delete the previous euler in the Object's quaternion if exists
	if(this->quaternion->euler != NULL)
		delete[] this->quaternion->euler;
	this->quaternion->euler =rotation;
	rotation->quaternion->setFromEuler(rotation,false);
}

void Object3D::setQuaternion(Quaternion* quaternion){
	this->quaternion = quaternion;
	//delete previous euler in new quaternion if exists
	/*if(quaternion->euler != NULL)
		delete[] (quaternion->euler);*/
	quaternion->euler= this->rotation;

	//delete the previous quaternion in Object's euler if exists
	if (this->rotation->quaternion != NULL)
		delete[] (this->rotation->quaternion);
	this->rotation->quaternion = quaternion;
	quaternion->euler->setFromQuaternion(quaternion,this->rotation->order,false);
}

void Object3D::setScale(Vec3* scale){
	this->scale = scale;
}

Mat4<GLfloat> * Object3D::getModelMatrix(){
	return this->modelMatrix;
}

void Object3D::updateModelMatrix(){
	Mat4<GLfloat> * translation = Mat4<GLfloat>::translationMatrix(
		this->getPosition()->getX(),
		this->getPosition()->getY(),
		this->getPosition()->getZ());
	Mat4<GLfloat> * rot = Mat4<GLfloat>::rotationMatrixFromQuaternion(
		this->getQuaternion());

	Mat4<GLfloat> * scale = Mat4<GLfloat>::scaleMatrix(
		this->getScale()->getX(),
		this->getScale()->getY(),
		this->getScale()->getZ());
	if(this->modelMatrix !=NULL)
		delete this->modelMatrix;
	this->modelMatrix = Mat4<GLfloat>::identityMatrix();
	
	this->modelMatrix->crossProduct(translation);
	this->modelMatrix->crossProduct(rot);
	this->modelMatrix->crossProduct(scale);

	delete scale;
	delete rot;
	delete translation;
}








