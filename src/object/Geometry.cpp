#include "object/Geometry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

Geometry::Geometry(){
	this->numMeshes =0;
	this->numVertices = 0;
	this->numElements = 0;
	this->numNormals = 0;
	this->vertices = NULL;
	this->elements = NULL;
	this->normals=NULL;
	this->vertexBuffer = 0;
	this->elementBuffer =0;
	this->normalBuffer =0;
	this->boundingBox =NULL;
}
GLfloat* Geometry::getVertices(){
	return this->vertices;
}
void Geometry::setVertices(GLfloat* vertices, int numVertices){
	this->numVertices = numVertices;
	this->vertices = vertices;
}
	
int Geometry::getNumVertices(){
	return this->numVertices;
}

int Geometry::getNumElements(){
	return this->numElements;
}

GLushort* Geometry::getElements(){
	return this->elements;
}

void Geometry::setElements(GLushort* elements, int numElements){
	this->elements = elements;
	this->numElements = numElements;
}

Geometry::~Geometry(){
	if(this->vertices != NULL)
		delete [] this->vertices;
	if(this->elements != NULL)
		delete [] this->elements;
	if(this->normals != NULL)
		delete [] this->normals;
	if(this->boundingBox != NULL){
		delete this->boundingBox;
	}
	glDeleteBuffers(1,&(this->elementBuffer));
	glDeleteBuffers(1,&(this->vertexBuffer));
	glDeleteBuffers(1,&(this->normalBuffer));
}

void Geometry::setNumMeshes(int numMeshes){
	this->numMeshes = numMeshes;
}

int Geometry::getNumMeshes(){
	return this->numMeshes;
}

void Geometry::requestDelete(){
	if(this->numMeshes == 0)
		this->~Geometry();
}

GLuint Geometry::getVertexBuffer(){
	return this->vertexBuffer;
}

void Geometry::setVertexBuffer(GLuint vertexBuffer){
	this->vertexBuffer = vertexBuffer;
}

GLuint Geometry::getElementBuffer(){
	return this->elementBuffer;
}

void Geometry::setElementBuffer(GLuint elementBuffer){
	this->elementBuffer = elementBuffer;
}

void exchangeNormalPosition(GLfloat* dest,GLfloat* src,GLuint destIndex,GLuint srcIndex){
	memcpy(&dest[3*destIndex],&src[3*srcIndex],sizeof(GLfloat)*3);
}

void Geometry::loadDataFromFile(const char* filename){//change to read without fopen & read .dae(collada files)
	FILE * data = fopen(filename,"r");
	if(data == NULL) return;
	//creating bounding box
	this->boundingBox = new struct bounds;
	this->boundingBox->x[0]=9999;
	this->boundingBox->x[1]=-9999;
	this->boundingBox->y[0]=9999;
	this->boundingBox->y[1]=-9999;
	this->boundingBox->z[0]=9999;
	this->boundingBox->z[1]=-9999;

	fscanf(data, "%d", &(this->numVertices));
	this->vertices = new GLfloat[this->numVertices];
	for(int i=0; i < this->numVertices;i+=3){
		float x,y,z;
		fscanf(data,"%f%f%f",&x,&y,&z);
		this->vertices[i] = x;
		this->vertices[i+1] = y;
		this->vertices[i+2] = z;
		this->boundingBox->x[0]=fmin(this->boundingBox->x[0],x);
		this->boundingBox->x[1]=fmax(this->boundingBox->x[1],x);
		this->boundingBox->y[0]=fmin(this->boundingBox->y[0],y);
		this->boundingBox->y[1]=fmax(this->boundingBox->y[1],y);
		this->boundingBox->z[0]=fmin(this->boundingBox->z[0],z);
		this->boundingBox->z[1]=fmax(this->boundingBox->z[1],z);
	}

	fscanf(data, "%d", &(this->numNormals));
	this->normals = new GLfloat[this->numNormals];
	for(int i=0; i < this->numNormals;i++){
		fscanf(data,"%f",&(this->normals[i]));
	}
	GLfloat orderedNormals[this->numNormals];
	fscanf(data, "%d", &(this->numElements));
    this->numElements *=3;
	this->elements = new GLushort[this->numElements];
	unsigned short int normalIndex;
	for(int i=0; i< this->numElements ;i++){
		fscanf(data,"%hu%hu",&(this->elements[i]),&normalIndex);
		exchangeNormalPosition(orderedNormals,this->normals,this->elements[i],normalIndex);
	}
	memcpy(this->normals,orderedNormals,sizeof(GLfloat)*this->numNormals);
	fclose(data);
}

GLfloat* Geometry::getNormals(){
	return this->normals;
}
void Geometry::setNormals(GLfloat* normals, int numNormals){
	this->normals = normals;
	this->numNormals = numNormals;
}
int Geometry::getNumNormals(){
	return this->numNormals;
}

GLuint Geometry::getNormalBuffer(){
	return this->normalBuffer;
}

void Geometry::setNormalBuffer(GLuint normalBuffer){
	this->normalBuffer = normalBuffer;
}

BoundingBox Geometry::getBoundingBox(){
	return this->boundingBox;
}

Geometry* Geometry::generateCubeGeometry(float size){
	float dist = size/2;
	int numVertices = 24;
	int numElements = 36;
	GLfloat* vertices = new GLfloat[numVertices];
	GLfloat* normals = new GLfloat[numVertices];
	GLushort* elements = new GLushort[numElements];
	for(int i=0; i < 8; i++){
		vertices[i*3] = dist * pow(-1,(i & 1 ? 1 :2));
		vertices[(i*3)+1] = dist * pow(-1,(i & 2 ? 1 :2));
		vertices[(i*3)+2] = dist * pow(-1,(i & 4 ? 1 :2));
		normals[i*3] = vertices[i*3] * 0.5773502;
		normals[(i*3)+1] = vertices[(i*3)+1] * 0.5773502;
		normals[(i*3)+2] = vertices[(i*3)+2] * 0.5773502;
	}

	BoundingBox boundingBox = new struct bounds;
	boundingBox->x[0]=vertices[21];
	boundingBox->x[1]=vertices[0];
	boundingBox->y[0]=vertices[22];
	boundingBox->y[1]=vertices[1];
	boundingBox->z[0]=vertices[23];
	boundingBox->z[1]=vertices[2];

	GLushort elementArray[36] = {0,1,4,1,5,4,7,5,4,7,4,6,4,0,6,0,2,6,1,5,7,1,3,7,1,7,0,1,0,2,7,3,6,6,3,2};
	memcpy(elements,elementArray, sizeof(GLushort)*36);
	Geometry * boxGeom = new Geometry();
	boxGeom->boundingBox = boundingBox;
	boxGeom->setVertices(vertices,numVertices);
	boxGeom->setElements(elements,numElements);
	boxGeom->setNormals(normals,numVertices);
	return boxGeom;
}
