#ifndef MAT4_H
#define MAT4_H
#include <math.h>
#include "math/Quaternion.h"

template <class T>
class Mat4{
private:
	T * elements;
	void setElements(T* elements){
		this->elements = elements;
	}
public:
	Mat4(T value){
		this->elements = new T[16];
		for(int i=0; i <16;i++){
			this->elements[i] = value;
		}
	};

	T * getElements(){
		return this->elements;
	}

	~Mat4(){
		if(this->getElements() != NULL)
			delete[] (this->getElements());
	}

	static Mat4<T> * identityMatrix(){
		Mat4<T> * mat = new Mat4<T>((T)0);
		mat->getElements()[0] =(T)1;
		mat->getElements()[5] =(T)1;
		mat->getElements()[10] =(T)1;
		mat->getElements()[15] =(T)1;
		return mat;
	}

	static Mat4<T> * translationMatrix(T x, T y , T z){
		Mat4<T> * mat = Mat4<T>::identityMatrix();
		mat->getElements()[3] =x;
		mat->getElements()[7] =y;
		mat->getElements()[11]=z;
		return mat;
	}

	static Mat4<T> * rotationMatrixFromQuaternion(Quaternion* q){
		Mat4<T> * mat = Mat4<T>::identityMatrix();
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

	static Mat4<T> * scaleMatrix(T x, T y , T z){
		Mat4<T> * mat = Mat4<T>::identityMatrix();
		mat->getElements()[0] =x;
		mat->getElements()[5] =y;
		mat->getElements()[10]=z;
		return mat;
	}

	void scalarProduct(T scalar){
		for(int i =0; i < 16 ; i++){
			this->elements[i] *= scalar;
		}
	}

	Mat4<T> * copy(){
		Mat4<T> * mat = new Mat4<T>((T)0);
		for(int i =0; i < 16; i++){
			mat->getElements[i] = this->getElements[i];
		}
		return mat;
	}

	static Mat4<T> * rotationMatrix(T x , T y, T z){
		T radX = 3.14159 * x /180.0;
		T radY = 3.14159 * y /180.0;
		T radZ = 3.14159 * z /180.0;

		Mat4<T> * rotX = Mat4<T>::identityMatrix();
		rotX->getElements()[5] = cos(radX);
		rotX->getElements()[6] = -sin(radX);
		rotX->getElements()[9] = sin(radX);
		rotX->getElements()[10] = cos(radX);

		Mat4<T> * rotY = Mat4<T>::identityMatrix();
		rotY->getElements()[0] = cos(radY);
		rotY->getElements()[2] = sin(radY);
		rotY->getElements()[8] = -sin(radY);
		rotY->getElements()[10] = cos(radY);

		Mat4<T> * rotZ = Mat4<T>::identityMatrix();
		rotZ->getElements()[0] = cos(radZ);
		rotZ->getElements()[1] = -sin(radZ);
		rotZ->getElements()[4] = sin(radZ);
		rotZ->getElements()[5] = cos(radZ);

		Mat4<T> * res = Mat4<T>::identityMatrix();
		res->crossProduct(rotX);
		res->crossProduct(rotY);
		res->crossProduct(rotZ);

		delete rotX;
		delete rotY;
		delete rotZ;

		return res;
	}

	static Mat4<T> * perspectiveMatrix(T fov, T aspectRatio, T zNear, T zFar){
		Mat4<T> * mat = new Mat4<T>((T)0);
		float radians = (float)fov * (3.14159 / 180.0);
		mat->getElements()[0] = (T)(1 / tan(radians));
		mat->getElements()[5] = aspectRatio * mat->getElements()[0];
		mat->getElements()[10] = (T)((zFar +zNear)/(zNear -zFar));
		mat->getElements()[11] = (T)((2.0 * zNear * zFar)/(zNear -zFar));
		mat->getElements()[14] = (T)-1.0;
		return mat;
	}

	static Mat4<T> * crossProductMatrices(Mat4<T> * m1, Mat4<T> * m2){
		Mat4<T> * mat = new Mat4<T>((T)0);
		T * temp = mat->getElements();
		T * a = m1->getElements();
		T * b = m2->getElements();
		
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

	void crossProduct(Mat4<T> * mat){
		//do this code more legible
		T * temp = new T[16];
		T * a = this->elements;
		T * b = mat->elements;

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

	Mat4<T> * getTraspose(){
		Mat4<T> * result = new Mat4<T>((T)0);
		T* temp = result->elements;
		T* a = this->elements;
		for(int i=0; i < 4; i++){
			for(int j=0; j < 4 ; j++){
				temp[i*4+j] = a[j*4+i];
			}
		}
		return result;
	}

	static Mat4<T>* lookAt(Vec3* eye, Vec3* target, Vec3* up){
		Mat4<T> * lookAt = Mat4<T>::identityMatrix();
		Vec3* zAxix = new Vec3();
		zAxis.setX(eye.getX()-target.getX());
		zAxis.setY(eye.getY()-target.getY());
		zAxis.setZ(eye.getZ()-target.getZ());
		zAxis.normalize();
		Vec3* xAxis = Vec3::crossProductVectors(zAxis,up);
		xAxis.normalize();
		Vec3* yAxix = Vec3::crossProductVectors(xAxis,zAxis);
		yAxis.normalize();

		T* mat = lookAt->elements;
		mat[0] = xAxis->getX();
		mat[1] = yAxis->getX();
		mat[2] = zAxis->getX();

		mat[4] = xAxis->getY();
		mat[5] = yAxis->getY();
		mat[6] = zAxis->getY();

		mat[8] = xAxis->getZ();
		mat[9] = yAxis->getZ();
		mat[10] = zAxis->getZ();

		delete yAxis;
		delete xAxis;
		delete zAxis;
		return lookAt;
	}
};

#endif