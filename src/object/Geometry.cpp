#include "object/Geometry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

	fscanf(data, "%d", &(this->numVertices));
	this->vertices = new GLfloat[this->numVertices];
	for(int i=0; i < this->numVertices;i++){
		fscanf(data,"%f",&(this->vertices[i]));
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
	unsigned short int color;
	for(int i=0; i< this->numElements ;i++){
		fscanf(data,"%hu%hu%hu",&(this->elements[i]),&normalIndex,&color);
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


