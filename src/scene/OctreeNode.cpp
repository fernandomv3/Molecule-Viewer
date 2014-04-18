#include "scene/OctreeNode.h"
#include "material/LineMaterial.h"
#include <string.h>

float OctreeNode::threshold =0.1;

OctreeNode::OctreeNode(){
	this->parent = NULL;
	this->size = 0;
	this->visible = true;
	this->boundingBox = NULL;
	this->position = NULL;
}

OctreeNode::OctreeNode(Vec3* position, float size){
	this->parent = NULL;
	this->size = size;
	this->visible = true;
	this->boundingBox = NULL;
	this->position = position;
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
	printf("starting boundingbox generation\n");
	if (this->boundingBox){
		printf("boundingbox already generated\n");
		return;
	} 

	float dist = this->size/2;;
	Vec3* center = this->getPosition();
	if(center == NULL)printf("NULL\n");

	int numVertices = 24;
	int numElements = 24;
	GLfloat* vertices = new GLfloat[numVertices];
	GLushort* elements = new GLushort[numElements];
	for(int i=0; i < 8; i++){
		vertices[i] = center->getX()+ dist * pow(-1,(i & 1 ? 2 :1));
		vertices[i+1] = center->getY()+ dist * pow(-1,(i & 1 ? 2 :1));
		vertices[i+2] = center->getZ()+ dist * pow(-1,(i & 1 ? 2 :1));
	}
	GLushort elementArray[24] = {0,1,0,2,0,4,1,3,1,5,2,3,2,6,3,7,4,5,4,6,5,7,6,7};
	memcpy(elements,elementArray, sizeof(GLushort)*24);
	Geometry * boxGeom = new Geometry();
	boxGeom->setVertices(vertices,numVertices);
	boxGeom->setElements(elements,numElements);
	LineMaterial* mat = new LineMaterial();
	mat->getDiffuseColor()->setRGB(0,0,0);
	this->boundingBox = new Mesh(boxGeom,mat);
	printf("bounding box generated\n");
}

void OctreeNode::setThreshold(float threshold){
	OctreeNode::threshold = threshold;
}

float OctreeNode::getThreshold(){
	return OctreeNode::threshold;
}

void OctreeNode::calculateVisibility(Camera* camera){

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
	if((bounds->x[0] > (center->getX() - dist)) && 
	   (bounds->x[1] < (center->getX() + dist)) &&
	   (bounds->y[0] > (center->getY() - dist)) && 
	   (bounds->y[1] < (center->getY() + dist)) &&
	   (bounds->z[0] > (center->getZ() - dist)) && 
	   (bounds->z[1] < (center->getZ() + dist))
	)
		return true;
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
	float childSize = this->size /2;
	for(int i = 0 ; i< 8; i++){
		float x = this->position->getX() + (childSize/2) * pow(-1,(i & 1 ? 2 :1));
		float y = this->position->getY() + (childSize/2) * pow(-1,(i & 2 ? 2 :1));
		float z = this->position->getZ() + (childSize/2) * pow(-1,(i & 4 ? 2 :1));
		Vec3* childPos = new Vec3(x,y,z);
		this->children.push_back(new OctreeNode(childPos,childSize));
	}
}

void OctreeNode::updateObjectPosition(Object3D* object){

}

void OctreeNode::addObject(Object3D* object){
	//fitting validation must be done before this method
	bool previouslyDivided = true;
	if(!this->isDivided()){
		this->subdivide();
		previouslyDivided = false;
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
	if(!previouslyDivided){
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

void OctreeNode::print(int indent){
	for(int i=0; i< indent;i++){
		printf("\t");
	}
	printf("node: address(%p)\t%d children\t%d objects\n",(void*)this,this->children.size(),this->objects.size());
	this->generateBoundingBox();
	OctreeNodeIterator node = this->children.begin();
	for(; node != this->children.end(); node++){
		(*node)->print(indent+1);
	}
}


