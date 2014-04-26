#include "math/Color.h"

Color::Color(){
	this->r = 1.0;
	this->g = 1.0;
	this->b = 1.0;
	this->a = 1.0;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0;
}


void Color::setRGB255(GLint r, GLint g, GLint b){
	this->r = r/255.0;
	this->g = g/255.0;
	this->b = b/255.0;
}

void Color::setRGB(GLfloat r, GLfloat g, GLfloat b){
	this->r = r;
	this->g = g;
	this->b = b;
}

void Color::setComponent(char component, GLfloat value){
	switch(component){
		case 'r':
			this->r = value;
			break;
		case 'g':
			this->g = value;
			break;
		case 'b':
			this->b = value;
			break; 
	}

}

void Color::addColor(Color * color){
	this->r += color->r;
	this->g += color->g;
	this->b += color->b;
}

GLfloat* Color::getAsArray(){
	GLfloat * col = new GLfloat[4];
	col[0] = this->r;
	col[1] = this->g;
	col[2] = this->b;
	col[3] = this->a;

	return col;
}

GLfloat Color::getComponent(char component){
	switch(component){
		case 'r':
			return this->r;
		case 'g':
			return this->g;
		case 'b':
			return this->b;
	}
	return (GLfloat)NULL;
}
