#include "math/SphericalCoord.h"

SphericalCoord::SphericalCoord(){
	this->phi=0;
	this->theta=0;
	this->r=0;
}

SphericalCoord::SphericalCoord(Vec3* cartesian, Vec3* relOrigin){
	this->phi=0;
	this->theta=0;
	this->r=0;
	this->setFromCartesian(cartesian,relOrigin);
}

float SphericalCoord::getPhi(){
	return this->phi;
}

float SphericalCoord::getTheta(){
	return this->theta;
}

float SphericalCoord::getR(){
	return this->r;
}

void SphericalCoord::setPhi(float phi){
	this->phi = phi;
}

void SphericalCoord::setTheta(float theta){
	this->theta = theta;
}

void SphericalCoord::setR(float r){
	this->r = r;
}

void SphericalCoord::setFromCartesian(Vec3 * position, Vec3* relOrigin){

	Vec3* sub = (relOrigin == NULL) ? position : Vec3::subVectors(position,relOrigin);
	float x2z2 = sqrt((sub->getX()*sub->getX())+(sub->getZ()*sub->getZ()));
	this->theta = atan2(sub->getX(),sub->getZ());
	this->phi = atan2(x2z2,sub->getY());
	this->r = sub->length();
	if(relOrigin != NULL) delete sub;

}

Vec3* SphericalCoord::getCartesian(Vec3* relOrigin){
	Vec3* position = new Vec3();
	position->setX(r * sin(this->phi) * sin(this->theta) + relOrigin->getX());
	position->setY(r * cos(this->phi) + relOrigin->getY());
	position->setZ(r * sin(this->phi) * cos(this->theta) + relOrigin->getZ());
	return position;
}
