#ifndef OCTREENODE_H
#define OCTREENODE_H

#include "object/Mesh.h"
#include "scene/Camera.h"
#include "math/Vec3.h"
#include <list>
using namespace std;

#define Object3DIterator list<Object3D*>::iterator
#define OctreeNodeIterator list<OctreeNode*>::iterator

class OctreeNode{
private:
	static float threshold;
	OctreeNode* parent;
	float size;
	Mesh* boundingBox;
	bool visible;
	Vec3* position;
	int level;
public:
	list<OctreeNode*> children;
	list<Object3D*> objects;
	OctreeNode();
	OctreeNode(Vec3* position, float size);
	~OctreeNode();
	OctreeNode* getParent();
	list<Object3D*> getObjects();
	void setParent(OctreeNode* parent);
	void setObjects(list<Object3D*> objects);
	float getSize();
	void setSize(float size);
	Mesh* getBoundingBox();
	void generateBoundingBox();
	static void setThreshold(float threshold);
	static float getThreshold();
	void calculateVisibility(Camera* camera);
	bool isVisible();
	bool isEmpty();
	int numObjects();
	bool objectFits(Object3D* object);
	void clearChildren();
	void subdivide();
	void updateObjectPosition(Object3D* object);
	void addObject(Object3D* object);
	Vec3* getPosition();
	void setPosition(Vec3* position);
	bool isDivided();
	list<OctreeNode*> getChildren();
	void generateTreeMesh();
	void print();
};


#endif