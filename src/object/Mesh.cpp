#include "object/Mesh.h"
#include <cstdlib>

Mesh::Mesh():Object3D(){
	this->geometry = NULL;
	this->material = NULL;
}

Mesh::Mesh(Geometry* geometry):Object3D(){
	this->geometry = geometry;
}

Mesh::Mesh(Geometry* geometry, Material* material):Object3D(){
	this->geometry = geometry;
	this->material = material;
}

Material * Mesh::getMaterial(){
	return this->material;
}

void Mesh::setMaterial(Material* material){
	this->material = material;
}


Geometry * Mesh::getGeometry(){
	return this->geometry;
}

void Mesh::setGeometry(Geometry* geometry){
	if (this->geometry != NULL) this->geometry->setNumMeshes(this->geometry->getNumMeshes()-1);//remove from prev geom
	this->geometry = geometry;
	if (this->geometry != NULL) this->geometry->setNumMeshes(this->geometry->getNumMeshes()+1);// add to new geom
}

Mesh::~Mesh(){
	Geometry * geom = this->geometry;
	this->setGeometry(NULL);
	geom->requestDelete();
	//material manager?
	//if (this->material != NULL) delete material;
}
