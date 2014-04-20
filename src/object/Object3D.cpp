#include "object/Object3D.h"
#include <cstdlib>
#include "scene/OctreeNode.h"

Object3D::Object3D(){
	this->visible = true;
	this->position = new Vec3(0,0,0);
	this->rotation = new Euler(0,0,0,"XYZ");
	this->quaternion = new Quaternion(0,0,0,1);
	this->scale = new Vec3(1,1,1);
	this->modelMatrix = Mat4::identityMatrix();
	this->octreeNode = NULL;
	this->rotation->quaternion = this->quaternion;
	this->quaternion->euler=this->rotation;
	this->parent = NULL;
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
	quaternion->euler= this->rotation;

	this->rotation->quaternion = quaternion;
	quaternion->euler->setFromQuaternion(quaternion,this->rotation->order,false);
}

void Object3D::setScale(Vec3* scale){
	this->scale = scale;
}

Mat4 * Object3D::getModelMatrix(){
	return this->modelMatrix;
}

void Object3D::updateModelMatrix(){
	Mat4 * translation = Mat4::translationMatrix(
		this->getPosition()->getX(),
		this->getPosition()->getY(),
		this->getPosition()->getZ());
	Mat4 * rot = Mat4::rotationMatrixFromQuaternion(
		this->getQuaternion());

	Mat4 * scale = Mat4::scaleMatrix(
		this->getScale()->getX(),
		this->getScale()->getY(),
		this->getScale()->getZ());
	if(this->modelMatrix !=NULL)
		delete this->modelMatrix;
	this->modelMatrix = Mat4::identityMatrix();
	
	this->modelMatrix->crossProduct(translation);
	this->modelMatrix->crossProduct(rot);
	this->modelMatrix->crossProduct(scale);

	delete scale;
	delete rot;
	delete translation;
}

bool Object3D::getVisible(){
	return this->visible 
	       && 
	       (this->octreeNode != NULL ? this->octreeNode->isVisible() : true);
}

void Object3D::setVisible(bool visible){
	this->visible = visible;
}

OctreeNode* Object3D::getOctreeNode(){
	return this->octreeNode;
}

void Object3D::setOctreeNode(OctreeNode* octreeNode){
	this->octreeNode = octreeNode;
}

Object3D* Object3D::getParent(){
	return this->parent;
}

void Object3D::setParent(Object3D* parent){
	this->parent = parent;
}

