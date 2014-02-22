#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "math/Euler.h"
#include "math/Quaternion.h"

GLfloat clamp(GLfloat num){
	return fmin(fmax(num,-1.0),1.0);
}

Euler::Euler(GLfloat x, GLfloat y, GLfloat z,const char * order){
	this->x = x;
	this->y = y;
	this->z = z;
	this->order = strdup(order);
	this->quaternion = NULL;
}

Euler::Euler(GLfloat x, GLfloat y, GLfloat z){
	this->x = x;
	this->y = y;
	this->z = z;
	this->order = strdup("XYZ");
	this->quaternion = NULL;
}

Euler::~Euler(){
	delete[] this->order;
}

char* Euler::getOrder(){
	return this->order;
}

GLfloat Euler::getX(){
	return this->x;
}
void Euler::setX(GLfloat x){
	this->x=x;
	if(this->quaternion!=NULL)
		this->quaternion->setFromEuler(this,false);
}
GLfloat Euler::getY(){
	return this->y;
}
void Euler::setY(GLfloat y){
	this->y=y;
	if(this->quaternion!=NULL)
		this->quaternion->setFromEuler(this,false);
}
GLfloat Euler::getZ(){
	return this->z;
}
void Euler::setZ(GLfloat z){
	this->z=z;
	if(this->quaternion!=NULL)
		this->quaternion->setFromEuler(this,false);
}

void Euler::setFromQuaternion(Quaternion* q, const char* order, bool update){
	GLfloat x2 = q->getX() * q->getX(), 
	        y2 = q->getY() * q->getY(), 
	        z2 = q->getZ() * q->getZ(), 
	        w2 = q->getW() * q->getW();

	if (!strcmp(order,"XYZ")){
		this->x = atan2( 2 * ( q->getX() * q->getW() - q->getY() * q->getZ() ), ( w2 - x2 - y2 + z2 ) );
        this->y = asin(  clamp( 2 * ( q->getX() * q->getZ() + q->getY() * q->getW() ) ) );
        this->z = atan2( 2 * ( q->getZ() * q->getW() - q->getX() * q->getY() ), ( w2 + x2 - y2 - z2 ) );

	}
	else if (!strcmp(order,"YXZ")){
		this->x = asin(  clamp( 2 * ( q->getX() * q->getW() - q->getY() * q->getZ() ) ) );
        this->y = atan2( 2 * ( q->getX() * q->getZ() + q->getY() * q->getW() ), ( w2 - x2 - y2 + z2 ) );
        this->z = atan2( 2 * ( q->getX() * q->getY() + q->getZ() * q->getW() ), ( w2 - x2 + y2 - z2 ) );
	}
	else if (!strcmp(order,"ZXY")){
		this->x = asin(  clamp( 2 * ( q->getX() * q->getW() + q->getY() * q->getZ() ) ) );
		this->y = atan2( 2 * ( q->getY() * q->getW() - q->getZ() * q->getX() ), ( w2 - x2 - y2 + z2 ) );
		this->z = atan2( 2 * ( q->getZ() * q->getW() - q->getX() * q->getY() ), ( w2 - x2 + y2 - z2 ) );
	}
	else if (!strcmp(order,"ZYX")){
		this->x = atan2( 2 * ( q->getX() * q->getW() + q->getZ() * q->getY() ), ( w2 - x2 - y2 + z2 ) );
        this->y = asin(  clamp( 2 * ( q->getY() * q->getW() - q->getX() * q->getZ() ) ) );
        this->z = atan2( 2 * ( q->getX() * q->getY() + q->getZ() * q->getW() ), ( w2 + x2 - y2 - z2 ) );
	}
	else if (!strcmp(order,"YZX")){
		this->x = atan2( 2 * ( q->getX() * q->getW() - q->getZ() * q->getY() ), ( w2 - x2 + y2 - z2 ) );
        this->y = atan2( 2 * ( q->getY() * q->getW() - q->getX() * q->getZ() ), ( w2 + x2 - y2 - z2 ) );
        this->z = asin(  clamp( 2 * ( q->getX() * q->getY() + q->getZ() * q->getW() ) ) );
	}
	else if (!strcmp(order,"XZY")){
		this->x = atan2( 2 * ( q->getX() * q->getW() + q->getY() * q->getZ() ), ( w2 - x2 + y2 - z2 ) );
        this->y = atan2( 2 * ( q->getX() * q->getZ() + q->getY() * q->getW() ), ( w2 + x2 - y2 - z2 ) );
        this->z = asin(  clamp( 2 * ( q->getZ() * q->getW() - q->getX() * q->getY() ) ) );
	}
	else{
		//incorrect order
		return;
	}
	delete this->order;
	this->order = strdup(order);

	update ? this->quaternion->setFromEuler(this,false) : (void)NULL;
}