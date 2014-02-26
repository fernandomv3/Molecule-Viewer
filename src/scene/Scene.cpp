#include "scene/Scene.h"
#include <algorithm>

using namespace std;

Scene::Scene(){
	this->camera = new Camera();
	Mat4* mat = this->camera->getProjectionMatrix(); 
	this->camera->setProjectionMatrix(Mat4::perspectiveMatrix(30.0, 4.0/3.0, 0.1, 100.0));
	delete mat;
	this->camera->getPosition()->setZ(4.0);
	this->ambientLight = new Light();
	this->ambientLight->getColor()->setRGB(0.25,0.25,0.25);
	this->directionalLightsUBO = 0;
	this->pointLightsUBO=0;
	this->ambientLightUBO = 0;
}

list<Object3D*> Scene::getObjects(){
	return this->objects;
}

list<Object3D*> Scene::getAddedObjects(){
	return this->addedObjects;
}

list<Object3D*> Scene::getRemovedObjects(){
	return this->removedObjects;
}

void Scene::addDirectionalLight(DirectionalLight* light){
	this->directionalLights.push_back(light);
}

void Scene::addObject(Object3D* o){
	if(find(this->objects.begin(),this->objects.end(), o) == this->objects.end()){
		this->objects.push_back(o);
		//this->addedObjects.push_back(o);

		list<Object3D*>::iterator it = find(this->removedObjects.begin(),this->removedObjects.end(),o);
		if( it  != this->removedObjects.end()){
			this->removedObjects.erase(it);
		}
	}
}


void Scene::removeObject(Object3D* o){
	list<Object3D*>::iterator it = find(this->objects.begin(),this->objects.end(), o);
	if(it != this->objects.end()){
		this->objects.erase(it);
		this->removedObjects.push_back(o);

		it = find(this->addedObjects.begin(),this->addedObjects.end(), o);
		if(it != this->addedObjects.end()){
			this->addedObjects.erase(it);
		}
	}
}

Camera * Scene::getCamera(){
	return this->camera;
}

void Scene::setCamera(Camera* camera){
	this->camera = camera;
}

Light* Scene::getAmbientLight(){
	return this->ambientLight;
}

void Scene::setAmbientLight(Light* ambientLight){
	this->ambientLight = ambientLight;
}

list<DirectionalLight*> Scene::getDirectionalLights(){
	return this->directionalLights;
}

GLuint Scene::getDirectionalLightsUBO(){
	return this->directionalLightsUBO;
}

GLuint Scene::getAmbientLightUBO(){
	return this->ambientLightUBO;
}

void Scene::setDirectionalLightsUBO(GLuint directionalLightsUBO){
	this->directionalLightsUBO = directionalLightsUBO;
}

void Scene::setAmbientLightUBO(GLuint ambientLightUBO){
	this->ambientLightUBO = ambientLightUBO;
}

list<PointLight*> Scene::getPointLights(){
	return this->pointLights;
}

void Scene::addPointLight(PointLight* pointLight){
	this->pointLights.push_back(pointLight);
}

GLuint Scene::getPointLightsUBO(){
	return this->pointLightsUBO;
}

void Scene::setPointLightsUBO(GLuint pointLightsUBO){
	this->pointLightsUBO = pointLightsUBO;
}


Scene::~Scene(){
	list<Object3D*> objects = this->objects;
	list<Object3D*>::iterator it = objects.begin();
	list<Object3D*>::iterator end = objects.end();
	for(;it != end;it++){
		delete (*it);
	}

	objects = this->removedObjects;
	it = objects.begin();
	end = objects.end();
	for(;it != end;it++){
		delete (*it);
	}
	objects = this->addedObjects;
	it = objects.begin();
	end = objects.end();
	for(;it != end;it++){
		delete (*it);
	}
	/*list<DirectionalLight*> lights = this->directionalLights;
	list<DirectionalLight*>::iterator itLights = lights.begin();
	list<DirectionalLight*>::iterator endLights = lights.end();
	for(;itLights != endLights;itLights++){
		delete (*itLights);
	}

	list<PointLight*> pointLights = this->pointLights;
	list<PointLight*>::iterator itPointLights = pointLights.begin();
	list<PointLight*>::iterator endPointLights = pointLights.end();
	for(;itPointLights != endPointLights;itPointLights++){
		delete (*itPointLights);
	}*/

	if (this->camera != NULL)
		delete (this->camera);
	if(this->ambientLight != NULL){
		delete (this->ambientLight);
	}
}
