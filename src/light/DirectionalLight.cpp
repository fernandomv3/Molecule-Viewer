#include "light/DirectionalLight.h"

DirectionalLight::DirectionalLight(){
	this->target = new Object3D();
	this->intensity = 1;
}

DirectionalLight::~DirectionalLight(){
	if(this->target != NULL)
		delete[] (this->target);
}

Object3D* DirectionalLight::getTarget(){
	return this->target;
}

GLfloat DirectionalLight::getIntensity(){
	return this->intensity;
}

void DirectionalLight::setIntensity(GLfloat intensity){
	this->intensity = intensity;
}

GLfloat* DirectionalLight::getVectorToLightAsArray(Camera* camera){
	GLfloat* vec = new GLfloat[4];
	Vec3* worldLightPos = this->getPosition()->applyMatrix(camera->getWorldMatrix(),1);
	Vec3* worldTargetPos = this->target->getPosition()->applyMatrix(camera->getWorldMatrix(),1);

	vec[0]= worldLightPos->getX() - worldTargetPos->getX();
	vec[1]= worldLightPos->getY() - worldTargetPos->getY();
	vec[2]= worldLightPos->getZ() - worldTargetPos->getZ();
	vec[3]= 0.0;
	delete worldLightPos;
	delete worldTargetPos;
	return vec;
}

DirLight DirectionalLight::getAsStruct(Camera* camera){
	DirLight light = new struct dirLight;
	light->intensity = this->intensity;
	GLfloat * color = this->getColor()->getAsArray();
	memcpy(&(light->color),color,sizeof(GLfloat)*4);
	delete[] color;
	GLfloat * vec = this->getVectorToLightAsArray(camera);
	memcpy(&(light->vectorToLight),vec,sizeof(GLfloat)*4);
	delete[] vec;
	return light;
}
