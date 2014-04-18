#include "object/Mesh.h"
#include <cstdlib>

Mesh::Mesh():Object3D(){
	this->geometry = NULL;
	this->material = NULL;
	this->boundingBox = NULL;
}

Mesh::Mesh(Geometry* geometry):Object3D(){
	this->geometry = geometry;
	this->boundingBox = NULL;
}

Mesh::Mesh(Geometry* geometry, Material* material):Object3D(){
	this->geometry = geometry;
	this->material = material;
	this->boundingBox = NULL;
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
	if (this->boundingBox != NULL)
		delete this->boundingBox;
	//material manager?
	//if (this->material != NULL) delete material;
}

BoundingBox Mesh::getBoundingBox(){
	this->updateBoundingBox();
	return this->boundingBox;
}

void Mesh::updateBoundingBox(){
	if(this->boundingBox == NULL){
		this->boundingBox = new struct bounds;
		this->boundingBox->x[0]=9999;
		this->boundingBox->x[1]=0;
		this->boundingBox->y[0]=9999;
		this->boundingBox->y[1]=0;
		this->boundingBox->z[0]=9999;
		this->boundingBox->z[1]=0;
	}
	Mat4* modelMatrix = this->getModelMatrix();
	Vec3 vertices[8];
	for(int i = 0; i < 8 ; i++){
		vertices[i].setX(this->getGeometry()->getBoundingBox()->x[(i & 1 ? 1 :0)]);
		vertices[i].setY(this->getGeometry()->getBoundingBox()->y[(i & 2 ? 1 :0)]);
		vertices[i].setZ(this->getGeometry()->getBoundingBox()->z[(i & 4 ? 1 :0)]);

		Vec3* newVert = vertices[i].applyMatrix(modelMatrix,1);
		this->boundingBox->x[0]=fmin(this->boundingBox->x[0],newVert->getX());
		this->boundingBox->x[1]=fmax(this->boundingBox->x[1],newVert->getX());
		this->boundingBox->y[0]=fmin(this->boundingBox->y[0],newVert->getY());
		this->boundingBox->y[1]=fmax(this->boundingBox->y[1],newVert->getY());
		this->boundingBox->z[0]=fmin(this->boundingBox->z[0],newVert->getZ());
		this->boundingBox->z[1]=fmax(this->boundingBox->z[1],newVert->getZ());
		delete newVert;
	}
}



