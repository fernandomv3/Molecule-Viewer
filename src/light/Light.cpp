#include "light/Light.h"

Light::Light(): Object3D(){
	this->color = new Color();
}
Light::~Light(){
	if (this->color != NULL) delete (this->color);
}
Color* Light::getColor(){
	return this->color;
}
void Light::setColor(Color* color){
	this->color = color;
}