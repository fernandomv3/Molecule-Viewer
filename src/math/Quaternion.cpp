#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "math/Quaternion.h"
#include "math/Euler.h"
#include "math/Vec3.h"
#include <cstdio>

Quaternion::Quaternion(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 1;
	this->euler = NULL;
}

Quaternion::Quaternion(GLfloat x, GLfloat y, GLfloat z,GLfloat w){
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	this->euler = NULL;
}

void Quaternion::setX(GLfloat x){
	this->x = x;
	if (this->euler != NULL)
		this->euler->setFromQuaternion(this,this->euler->order,false);
}

void Quaternion::setY(GLfloat y){
	this->y = y;
	if (this->euler != NULL)
		this->euler->setFromQuaternion(this,this->euler->order,false);
}

void Quaternion::setZ(GLfloat z){
	this->z = z;
	if (this->euler != NULL)
		this->euler->setFromQuaternion(this,this->euler->order,false);
}

void Quaternion::setW(GLfloat w){
	this->w = w;
	if (this->euler != NULL)
		this->euler->setFromQuaternion(this,this->euler->order,false);
}

GLfloat Quaternion::getX(){
	return this->x;
}

GLfloat Quaternion::getY(){
	return this->y;
}

GLfloat Quaternion::getZ(){
	return this->z;
}

GLfloat Quaternion::getW(){
	return this->w;
}

void Quaternion::setComponent(int index, GLfloat value){
	switch(index){
		case 0:
			this->x = value;
			break;
		case 1:
			this->y = value;
			break;
		case 2:
			this->z = value;
			break;
		case 3:
		    this->w = value;
		    break;
	}
	if (this->euler != NULL)
		this->euler->setFromQuaternion(this,this->euler->order,false);
}

GLfloat Quaternion::getComponent(int index){
	switch(index){
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		case 3:
		    return this->w;
	}
	return (GLfloat)NULL;
}

void Quaternion::setFromEuler(Euler* euler , bool update){
	GLfloat c1 = cos( euler->getX() / 2 ),
            c2 = cos( euler->getY() / 2 ),
            c3 = cos( euler->getZ() / 2 ),
            s1 = sin( euler->getX() / 2 ),
            s2 = sin( euler->getY() / 2 ),
            s3 = sin( euler->getZ() / 2 );

    if ( !strcmp(euler->getOrder(), "XYZ") ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( !strcmp(euler->getOrder(), "YXZ") ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( !strcmp(euler->getOrder(), "ZXY") ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( !strcmp(euler->getOrder(), "ZYX") ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( !strcmp(euler->getOrder(), "YZX") ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( !strcmp(euler->getOrder(), "XZY") ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;
    }

    update ? this->euler->setFromQuaternion(this,euler->order,false) : (void)NULL;
}

GLfloat Quaternion::length(){
	return sqrt((this->x * this->x)+(this->y * this->y)+(this->z * this->z)+(this->w * this->w));
}

void Quaternion::normalize(){
	GLfloat l = this->length();
	if(l==0){
		this->x=0;
		this->y=0;
		this->z=0;
		this->w=1;
	}
	else{
		l= 1/l;
		this->x*=l;
		this->y*=l;
		this->z*=l;
		this->w*=l;
	}
}

Quaternion * Quaternion::conjugate(){
	Quaternion* q = new Quaternion();
	q->euler = this->euler;
	q->x = this->x * -1;
	q->y = this->y * -1;
	q->z = this->z * -1;
	q->w = this->w;
	//q->euler->setFromQuaternion(q,euler->order,false);
	return q;
}

Quaternion * Quaternion::inverse(){
	Quaternion* q = this->conjugate();
	q->normalize();
	return q;
}

Quaternion* Quaternion::multiplyQuaternions(Quaternion* q1, Quaternion* q2){
	Quaternion* quat = new Quaternion();
	quat->x = q1->x * q2->w + q1->w * q2->x + q1->y * q2->z - q1->z * q2->y;
	quat->y = q1->y * q2->w + q1->w * q2->y + q1->z * q2->x - q1->x * q2->z;
	quat->z = q1->z * q2->w + q1->w * q2->z + q1->x * q2->y - q1->y * q2->x;
	quat->w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
	if (quat->euler != NULL)
		quat->euler->setFromQuaternion(quat,quat->euler->order,false);
	return quat;
}

void  Quaternion::multiply(Quaternion* q){
	this->x = this->x * q->w + this->w * q->x + this->y * q->z - this->z * q->y;
	this->y = this->y * q->w + this->w * q->y + this->z * q->x - this->x * q->z;
	this->z = this->z * q->w + this->w * q->z + this->x * q->y - this->y * q->x;
	this->w = this->w * q->w - this->x * q->x - this->y * q->y - this->z * q->z;
	if (this->euler != NULL)
		this->euler->setFromQuaternion(this,euler->order,false);
}

GLfloat Quaternion::dotProduct(Quaternion* q){
	return this->x * q->x + this->y * q->y + this->z * q->z + this->w * q->w;
}

GLfloat Quaternion::dotProductQuaternion(Quaternion* q1, Quaternion* q2){
	return q1->x * q2->x + q1->y * q2->y + q1->z * q2->z + q1->w * q2->w;
}

Quaternion* Quaternion::rotationBetweenVectors(Vec3* vec1, Vec3* vec2){
	GLfloat e = vec1->dotProduct(vec2);
	Vec3* vec = Vec3::crossProductVectors(vec1,vec2);
	if(vec == NULL) printf("Hola\n");
	GLfloat term = sqrt(2*(1+e));
	Quaternion* q = new Quaternion();
	q->setX(vec->getX() * 1.0/term);
	q->setY(vec->getY() * 1.0/term);
	q->setZ(vec->getZ() * 1.0/term);
	q->setW(term / 2.0);
	delete vec;
	return q;
}

void Quaternion::setEuler(Euler* euler){
	this->euler = euler;
}

void Quaternion::setFromMat4(Mat4<GLfloat> mat4){

}
