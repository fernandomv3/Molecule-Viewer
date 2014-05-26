#include "material/Material.h"
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <scene/Scene.h>
using namespace std;

Material::Material(){
	this->diffuseColor = new Color();
	this->specularColor = new Color();
	this->shininess = 1;
	this->vertexShaderSource = NULL;
	this->fragmentShaderSource = NULL;
	this->program = NULL;
	this->sceneIndex = -1;
}

GLProgram* Material::getProgram(){
	return this->program;
}

void Material::setProgram(GLProgram* program){
	this->program = program;
}

void Material::setDiffuseColor(Color* color){
	this->diffuseColor = color;
}

Color* Material::getDiffuseColor(){
	return this->diffuseColor;
}

GLchar* Material::getVertexShaderSource(){
	return this->vertexShaderSource;
}

void Material::setVertexShaderSource(char * vertexShaderSource){
	this->vertexShaderSource = vertexShaderSource;
}

GLchar* Material::getFragmentShaderSource(){
	return this->fragmentShaderSource;
}

void Material::setFragmentShaderSource(char * fragmentShaderSource){
	this->fragmentShaderSource = fragmentShaderSource;
}

void Material::setSpecularColor(Color* color){
	this->specularColor = color;
}

Color* Material::getSpecularColor(){
	return this->specularColor;
}

void Material::setShininess(GLfloat shininess){
	this->shininess = shininess;
}

GLfloat Material::getShininess(){
	return this->shininess;
}

MaterialStruct Material::getAsStruct(){
	MaterialStruct material = new struct materialStruct;
	GLfloat * diffuseColor = this->getDiffuseColor()->getAsArray();
	memcpy(&(material->diffuseColor),diffuseColor,sizeof(GLfloat)*4);
	delete diffuseColor;
	GLfloat * specularColor = this->getSpecularColor()->getAsArray();
	memcpy(&(material->specularColor),specularColor,sizeof(GLfloat)*4);
	delete specularColor;
	material->shininess = this->shininess;
	return material;
}


Material::~Material(){
	if(this->diffuseColor != NULL){
		//delete this->diffuseColor;
	}
	if(this->specularColor != NULL){
		delete this->specularColor;
	}
	if(this->vertexShaderSource != NULL){
		delete this->vertexShaderSource;
	}
	if(this->fragmentShaderSource != NULL){
		delete this->fragmentShaderSource;
	}
	if(this->program != NULL){
		delete this->program;
	}	
}

MaterialType Material::getType(){
	return this->type;
}

int Material::getSceneIndex(){
	return this->sceneIndex;
}

void Material::setSceneIndex(int index){
	this->sceneIndex = index;
}


char* Material::configureSource(char* source,int numDirLights , int numPointLights, int numObjects, int numMaterials){
	char src[strlen(source)+100];
	if( numMaterials != 0){
		sprintf(src,source,numObjects,numMaterials,numDirLights,numPointLights);
	}
	else{
		sprintf(src,source,numObjects,numDirLights,numPointLights);
	}
	delete source;
	return strdup(src);
}

void Material::makePrograms(Scene* scene){
	
}