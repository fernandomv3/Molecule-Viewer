#include "math/Vec3.h"
#include <cstdlib>

Vec3::Vec3(){

}

Vec3::Vec3(GLfloat x,GLfloat y,GLfloat z){
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vec3::setX(GLfloat x){
	this->x = x;
}
void Vec3::setY(GLfloat y){
	this->y = y;
}
void Vec3::setZ(GLfloat z){
	this->z = z;
}
GLfloat Vec3::getX(){
	return this->x;
}
GLfloat Vec3::getY(){
	return this->y;
}
GLfloat Vec3::getZ(){
	return this->z;
}
void Vec3::setComponent(int index, GLfloat value){
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
	}
}
GLfloat Vec3::getComponent(int index){
	switch(index){
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
	}
	return (GLfloat)NULL;
}

Vec3* Vec3::applyMatrix(Mat4<GLfloat>* matrix, GLfloat w){
	Vec3* vector = new Vec3();
	GLfloat* mat = matrix->getElements();
	vector->x = mat[0]*this->x + mat[1]*this->y + mat[2] *this->z + mat[3]*w;
	vector->y = mat[4]*this->x + mat[5]*this->y + mat[6] *this->z + mat[7]*w;
	vector->z = mat[8]*this->x + mat[9]*this->y + mat[10] *this->z + mat[11]*w;
	return vector;
}

void Vec3::crossProduct(Vec3* vec){
	this->x = this->y * vec->z - this->z * vec->y;
	this->y = this->z * vec->x - this->x * vec->z;
	this->z = this->x * vec->y - this->y * vec->x;
}

Vec3* Vec3::crossProductVectors(Vec3* v1, Vec3* v2){
	Vec3* vec = new Vec3();
	vec->x = v1->y * v2->z - v1->z * v2->y;
	vec->y = v1->z * v2->x - v1->x * v2->z;
	vec->z = v1->x * v2->y - v1->y * v2->x;
	return vec;
}

GLfloat Vec3::dotProduct(Vec3* vec){
	return this->x * vec->x + this->y * vec->y + this->z * vec->z;
}

GLfloat Vec3::distance(Vec3* vec){
	GLfloat x2 = (vec->x - this->x)*(vec->x - this->x);
	GLfloat y2 = (vec->y - this->y)*(vec->y - this->y);
	GLfloat z2 = (vec->z - this->z)*(vec->z - this->z);
	return sqrt(x2 + y2 + z2);
}

void Vec3::normalize(){
	GLfloat l = this->length();
	if(l==0){
		this->x=0;
		this->y=0;
		this->z=0;
	}
	else{
		l= 1/l;
		this->x*=l;
		this->y*=l;
		this->z*=l;
	}
}

GLfloat Vec3::length(){
	return sqrt((this->x * this->x)+(this->y * this->y)+(this->z * this->z));
}
