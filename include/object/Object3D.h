#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <GL/glew.h>
#include "math/Vec3.h"
#include "math/Mat4.h"
#include "math/Quaternion.h"
#include "math/Euler.h"
#include <list>

using namespace std;

class OctreeNode;

class Object3D{
private:
	Vec3* position;
	Euler* rotation;
	Quaternion* quaternion;
	Vec3* scale;
	Mat4 * modelMatrix;
	bool visible;
	OctreeNode* octreeNode;
	Object3D* parent;
public:
	list<Object3D*> objects;
	Object3D();
	virtual ~Object3D();
	Vec3* getPosition();
	Euler* getRotation();
	Vec3* getScale();
	void setPosition(Vec3* position);
	void setRotation(Euler* rotation);
	void setScale(Vec3* scale);
	Mat4 * getModelMatrix();
	void updateModelMatrix();
	void setQuaternion(Quaternion* quaternion);
	Quaternion* getQuaternion();
	bool getVisible();
	void setVisible(bool visible);
	OctreeNode* getOctreeNode();
	void setOctreeNode(OctreeNode* octreeNode);
	Object3D* getParent();
	void setParent(Object3D* parent);
	void updateOctreeNode();
};

#endif