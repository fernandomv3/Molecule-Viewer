#include "light/PointLight.h"
#include <stdio.h>

PointLight::PointLight():Light(){
	this->intensity = 1;
	this->attenuation = 0.3;
}

void PointLight::setIntensity(GLfloat intensity){
	this->intensity = intensity;
}

GLfloat PointLight::getIntensity(){
	return this->intensity;
}

PointLight::~PointLight(){
	
}

void PointLight::setAttenuation(GLfloat attenuation){
	this->attenuation = attenuation;
}

GLfloat PointLight::getAttenuation(){
	return this->attenuation;
}


PLight PointLight::getAsStruct(Camera* camera){
	PLight light = new struct pLight;
	light->intensity = this->intensity;
	light->attenuation = this->attenuation;
	GLfloat * color = this->getColor()->getAsArray();
	memcpy(&(light->color),color,sizeof(GLfloat)*4);
	delete[] color;

	Vec3 * worldSpacePosition = this->getPosition()->applyMatrix(camera->getWorldMatrix(),1);
	light->position[0] = worldSpacePosition->getX();
	light->position[1] = worldSpacePosition->getY();
	light->position[2] = worldSpacePosition->getZ();
	light->position[3] = 1.0;

	delete worldSpacePosition;
	return light;
}