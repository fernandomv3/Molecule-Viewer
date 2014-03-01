#include <math.h>
#include "math/Quaternion.h"
#include "math/Vec3.h"
#include "math/Mat4.h"


void Mat4::setElements(GLfloat* elements){
	this->elements = elements;
}
Mat4::Mat4(GLfloat value){
	this->elements = new GLfloat[16];
	for(int i=0; i <16;i++){
		this->elements[i] = value;
	}
};

GLfloat * Mat4::getElements(){
	return this->elements;
}

Mat4::~Mat4(){
	if(this->getElements() != NULL)
		delete[] (this->getElements());
}

Mat4 * Mat4::identityMatrix(){
	Mat4 * mat = new Mat4(0);
	mat->getElements()[0] =1;
	mat->getElements()[5] =1;
	mat->getElements()[10] =1;
	mat->getElements()[15] =1;
	return mat;
}

Mat4 * Mat4::translationMatrix(GLfloat x, GLfloat y , GLfloat z){
	Mat4 * mat = Mat4::identityMatrix();
	mat->getElements()[3] =x;
	mat->getElements()[7] =y;
	mat->getElements()[11]=z;
	return mat;
}

Mat4 * Mat4::rotationMatrixFromQuaternion(Quaternion* q){
	Mat4 * mat = Mat4::identityMatrix();
	q->normalize();
	mat->getElements()[0] = 1-(2*q->getY()*q->getY())-(2*q->getZ()*q->getZ());
	mat->getElements()[1] = (2*q->getX()*q->getY())-(2*q->getW()*q->getZ());
	mat->getElements()[2] = (2*q->getX()*q->getZ())+(2*q->getW()*q->getY());
	mat->getElements()[4] = (2*q->getX()*q->getY())+(2*q->getW()*q->getZ());
	mat->getElements()[5] = 1-(2*q->getX()*q->getX())-(2*q->getZ()*q->getZ());
	mat->getElements()[6] = (2*q->getY()*q->getZ())-(2*q->getW()*q->getX());
	mat->getElements()[8] = (2*q->getX()*q->getZ())-(2*q->getW()*q->getY());
	mat->getElements()[9] = (2*q->getY()*q->getZ())+(2*q->getW()*q->getX());
	mat->getElements()[10] = 1-(2*q->getX()*q->getX())-(2*q->getY()*q->getY());
	return mat;
}

Mat4 * Mat4::scaleMatrix(GLfloat x, GLfloat y , GLfloat z){
	Mat4 * mat = Mat4::identityMatrix();
	mat->getElements()[0] =x;
	mat->getElements()[5] =y;
	mat->getElements()[10]=z;
	return mat;
}

void Mat4::scalarProduct(GLfloat scalar){
	for(int i =0; i < 16 ; i++){
		this->elements[i] *= scalar;
	}
}

Mat4 * Mat4::copy(){
	Mat4 * mat = new Mat4(0);
	for(int i =0; i < 16; i++){
		mat->getElements()[i] = this->getElements()[i];
	}
	return mat;
}

Mat4 * Mat4::rotationMatrix(GLfloat x , GLfloat y, GLfloat z){
	GLfloat radX = 3.14159 * x /180.0;
	GLfloat radY = 3.14159 * y /180.0;
	GLfloat radZ = 3.14159 * z /180.0;

	Mat4 * rotX = Mat4::identityMatrix();
	rotX->getElements()[5] = cos(radX);
	rotX->getElements()[6] = -sin(radX);
	rotX->getElements()[9] = sin(radX);
	rotX->getElements()[10] = cos(radX);

	Mat4 * rotY = Mat4::identityMatrix();
	rotY->getElements()[0] = cos(radY);
	rotY->getElements()[2] = sin(radY);
	rotY->getElements()[8] = -sin(radY);
	rotY->getElements()[10] = cos(radY);

	Mat4 * rotZ = Mat4::identityMatrix();
	rotZ->getElements()[0] = cos(radZ);
	rotZ->getElements()[1] = -sin(radZ);
	rotZ->getElements()[4] = sin(radZ);
	rotZ->getElements()[5] = cos(radZ);

	Mat4 * res = Mat4::identityMatrix();
	res->crossProduct(rotX);
	res->crossProduct(rotY);
	res->crossProduct(rotZ);

	delete rotX;
	delete rotY;
	delete rotZ;

	return res;
}

Mat4 * Mat4::perspectiveMatrix(GLfloat fov, GLfloat aspectRatio, GLfloat zNear, GLfloat zFar){
	Mat4 * mat = new Mat4(0);
	float radians = (float)fov * (3.14159 / 180.0);
	mat->getElements()[0] = (1 / tan(radians));
	mat->getElements()[5] = aspectRatio * mat->getElements()[0];
	mat->getElements()[10] = ((zFar +zNear)/(zNear -zFar));
	mat->getElements()[11] = ((2.0 * zNear * zFar)/(zNear -zFar));
	mat->getElements()[14] = -1.0;
	return mat;
}

Mat4 * Mat4::crossProductMatrices(Mat4 * m1, Mat4 * m2){
	Mat4 * mat = new Mat4(0);
	GLfloat * temp = mat->getElements();
	GLfloat * a = m1->getElements();
	GLfloat * b = m2->getElements();
	
	temp[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
    temp[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
    temp[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
    temp[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

    temp[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
    temp[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
    temp[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
    temp[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

    temp[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
    temp[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
    temp[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
    temp[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

    temp[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
    temp[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
    temp[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
    temp[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
	
	return mat;
}

void Mat4::crossProduct(Mat4 * mat){
	//do this code more legible
	GLfloat * temp = new GLfloat[16];
	GLfloat * a = this->elements;
	GLfloat * b = mat->elements;

	temp[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
    temp[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
    temp[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
    temp[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

    temp[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
    temp[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
    temp[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
    temp[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

    temp[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
    temp[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
    temp[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
    temp[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

    temp[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
    temp[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
    temp[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
    temp[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];


	delete[] (this->getElements());
	this->setElements(temp);
}

Mat4 * Mat4::getTraspose(){
	Mat4 * result = new Mat4(0);
	GLfloat* temp = result->elements;
	GLfloat* a = this->elements;
	for(int i=0; i < 4; i++){
		for(int j=0; j < 4 ; j++){
			temp[i*4+j] = a[j*4+i];
		}
	}
	return result;
}

Mat4* Mat4::lookAt(Vec3* eye, Vec3* target, Vec3* up){
	Mat4 * lookAt = Mat4::identityMatrix();

	Vec3* zAxis = new Vec3();
	zAxis->setX(eye->getX()-target->getX());
	zAxis->setY(eye->getY()-target->getY());
	zAxis->setZ(eye->getZ()-target->getZ());
	zAxis->normalize();
	if(zAxis->length()==0){
		zAxis->setZ(1);
	}

	Vec3* xAxis = Vec3::crossProductVectors(up,zAxis);
	xAxis->normalize();
	if(xAxis->length() == 0){
		zAxis->setX(zAxis->getX()+0.0001);
		delete xAxis;
		xAxis = Vec3::crossProductVectors(up,zAxis);
		xAxis->normalize();
	}

	Vec3* yAxis = Vec3::crossProductVectors(zAxis,xAxis);


	GLfloat* mat = lookAt->elements;
	mat[0] = xAxis->getX();
	mat[4] = yAxis->getX();
	mat[8] = zAxis->getX();

	mat[1] = xAxis->getY();
	mat[5] = yAxis->getY();
	mat[9] = zAxis->getY();

	mat[2] = xAxis->getZ();
	mat[6] = yAxis->getZ();
	mat[10] = zAxis->getZ();

	delete yAxis;
	delete xAxis;
	delete zAxis;
	return lookAt;
}
