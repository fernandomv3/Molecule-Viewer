#include "scene/OctreeNode.h"
#include "material/PhongMaterial.h"
#include "material/GouraudMaterial.h"
#include "material/LineMaterial.h"
#include <string.h>

float OctreeNode::threshold =0.1;

OctreeNode::OctreeNode(){
	this->parent = NULL;
	this->size = 40;
	this->visible = true;
	this->boundingBox = NULL;
	this->position = NULL;
	this->level= 0;
}

OctreeNode::OctreeNode(Vec3* position, float size){
	this->parent = NULL;
	this->size = size;
	this->visible = true;
	this->boundingBox = NULL;
	this->position = position;
	this->level =0;
}

OctreeNode::~OctreeNode(){
	delete this->boundingBox;
	delete this->position;
}

OctreeNode* OctreeNode::getParent(){
	return this->parent;
}

list<Object3D*> OctreeNode::getObjects(){
	return this->objects;
}

void OctreeNode::setParent(OctreeNode* parent){
	this->parent = parent;
}

void OctreeNode::setObjects(list<Object3D*> objects){
	this->objects = objects;
}

float OctreeNode::getSize(){
	return this->size;
}

void OctreeNode::setSize(float size){
	this->size = size;
}

Mesh* OctreeNode::getBoundingBox(){
	return this->boundingBox;
}

void OctreeNode::generateBoundingBox(){
	Geometry* box;
	LineMaterial* mat;
	float newScale=1;
	if(this->parent == NULL || 
	   this->parent->boundingBox == NULL || 
	   this->parent->boundingBox->getGeometry() == NULL){ 
		box = Geometry::generateCubeWireframe(this->size);
		mat = new LineMaterial();
	}
	else{
		box = this->parent->boundingBox->getGeometry();
		mat = (LineMaterial*)this->parent->boundingBox->getMaterial();
		newScale = 0.5/(this->level);
	}
	mat->getDiffuseColor()->setRGB(0,0,0);
	Mesh* cube = new Mesh(box,mat);
	cube->getScale()->setX(newScale);
	cube->getScale()->setY(newScale);
	cube->getScale()->setZ(newScale);
	cube->getPosition()->setX(this->getPosition()->getX());
	cube->getPosition()->setY(this->getPosition()->getY());
	cube->getPosition()->setZ(this->getPosition()->getZ());

	this->boundingBox = cube;
}

void OctreeNode::setThreshold(float threshold){
	OctreeNode::threshold = threshold;
}

float OctreeNode::getThreshold(){
	return OctreeNode::threshold;
}

void OctreeNode::calculateVisibility(Camera* camera){
	BoundingBox bounds = new struct bounds;
	Mesh* cube = this->getBoundingBox();
	Mat4 * worldTraspose = camera->getWorldMatrix()->getTraspose();
	Mat4 * projectionTraspose = camera->getProjectionMatrix()->getTraspose();
	Vec3 vertices[8];
	for(int i = 0; i < 8 ; i++){
		vertices[i].setX(cube->getGeometry()->getBoundingBox()->x[(i & 1 ? 1 :0)]);
		vertices[i].setY(cube->getGeometry()->getBoundingBox()->y[(i & 2 ? 1 :0)]);
		vertices[i].setZ(cube->getGeometry()->getBoundingBox()->z[(i & 4 ? 1 :0)]);

		Vec3* worldVert = vertices[i].applyMatrix(worldTraspose,1);
		Vec3* newVert = worldVert->applyMatrix(projectionTraspose,1);
		delete worldVert;
		bounds->x[0]=fmin(bounds->x[0],newVert->getX());
		bounds->x[1]=fmax(bounds->x[1],newVert->getX());
		bounds->y[0]=fmin(bounds->y[0],newVert->getY());
		bounds->y[1]=fmax(bounds->y[1],newVert->getY());
		bounds->z[0]=fmin(bounds->z[0],newVert->getZ());
		bounds->z[1]=fmax(bounds->z[1],newVert->getZ());
		delete newVert;
	}
	if(bounds->x[0] < -1 || bounds->y[0] < -1 || bounds->z[0] < -1 ||
	   bounds->x[1] > 1 || bounds->y[1] > 1 || bounds->z[1] > 1 ){
		this->visible = true;
		OctreeNodeIterator node = this->children.begin();
		for(; node != this->children.end(); node++){
			(*node)->calculateVisibility(camera);
		}
	}
}

bool OctreeNode::isVisible(){
	return this->visible;
}

bool OctreeNode::isEmpty(){
	return this->objects.empty();
}

int OctreeNode::numObjects(){
	return this->objects.size();
}

bool OctreeNode::objectFits(Object3D* object){
	float dist = this->size/2 + OctreeNode::threshold;
	Vec3* center = this->getPosition();
	BoundingBox bounds = ((Mesh*)object)->getBoundingBox();
	if((bounds->x[0] >= (center->getX() - dist)) && 
	   (bounds->x[1] <= (center->getX() + dist)) &&
	   (bounds->y[0] >= (center->getY() - dist)) && 
	   (bounds->y[1] <= (center->getY() + dist)) &&
	   (bounds->z[0] >= (center->getZ() - dist)) && 
	   (bounds->z[1] <= (center->getZ() + dist))
	){
		return true;
		printf("fits!\n");
	}
	return false;
}

void OctreeNode::clearChildren(){
	OctreeNodeIterator node = this->children.begin();
	for(; node != this->children.end(); node++){
		Object3DIterator obj = (*node)->objects.begin();
		Object3DIterator end = (*node)->objects.end();
		for(;obj != end ;obj++){
			(*obj)->setOctreeNode(NULL);
		}
		(*node)->objects.clear();
		(*node)->clearChildren();
		delete (*node);
	}
	this->children.clear();
}

void OctreeNode::subdivide(){
	//if(this->level > 1) return;
	float childSize = this->size /2;
	for(int i = 0 ; i< 8; i++){
		float x = this->position->getX() + (childSize/2) * pow(-1,(i & 1 ? 1 :2));
		float y = this->position->getY() + (childSize/2) * pow(-1,(i & 2 ? 1 :2));
		float z = this->position->getZ() + (childSize/2) * pow(-1,(i & 4 ? 1 :2));
		Vec3* childPos = new Vec3(x,y,z);
		OctreeNode* child = new OctreeNode(childPos,childSize);
		child->parent = this;
		child->level = this->level +1;
		this->children.push_back(child);
	}
}

void OctreeNode::updateObjectPosition(Object3D* object){

	OctreeNode* actualNode = object->getOctreeNode();
	actualNode->objects.remove(object);
	if (actualNode->objectFits(object)){
		actualNode->addObject(object);
		return;
	}
	OctreeNode* parent =actualNode->getParent();
	if(parent == NULL){
		actualNode->objects.push_back(object);
	}
	object->setOctreeNode(actualNode->getParent());
	updateObjectPosition(object);
}

void OctreeNode::addObject(Object3D* object){
	//fitting validation must be done before this method
	bool previouslySubdivided = true;
	if(!this->isDivided()){
		this->subdivide();
		previouslySubdivided = false;
	}
	OctreeNodeIterator node = this->children.begin();
	for(; node != this->children.end(); node++){
		if((*node)->objectFits(object)){
			(*node)->addObject(object);
			return;
		}
	}
	object->setOctreeNode(this);
	this->objects.push_back(object);
	if(!previouslySubdivided){
		this->clearChildren();
	} 
	return;
}

Vec3* OctreeNode::getPosition(){
	return this->position;
}

void OctreeNode::setPosition(Vec3* position){
	this->position = position;
}

bool OctreeNode::isDivided(){
	return !this->children.empty();
}

list<OctreeNode*> OctreeNode::getChildren(){
	return this->children;
}

void OctreeNode::generateTreeMesh(){
	this->generateBoundingBox();
	OctreeNodeIterator node = this->children.begin();
	for(; node != this->children.end(); node++){
		(*node)->generateTreeMesh();
	}
}

void OctreeNode::print(){
	for(int i=0; i< this->level;i++){
		printf("  ");
	}
	printf("node %p\t%d\n", (void*)(this),this->objects.size());
	OctreeNodeIterator node = this->children.begin();
	for(; node != this->children.end(); node++){
		(*node)->print();
	}
}

int OctreeNode::objectsInBranch(){
	int objectsInCurrentNode= this->objects.size();
	OctreeNodeIterator node = this->children.begin();
	for(; node != this->children.end(); node++){
		objectsInCurrentNode += (*node)->objectsInBranch();
	}
	return objectsInCurrentNode;
}

