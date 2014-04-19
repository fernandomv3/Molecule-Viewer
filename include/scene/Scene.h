#ifndef SCENE_H
#define SCENE_H
#include <cstdlib>
#include <list>
#include "object/Object3D.h"
#include "math/Mat4.h"
#include "scene/Camera.h"
#include "light/Light.h"
#include "light/DirectionalLight.h"
#include "light/PointLight.h"
#include "scene/OctreeNode.h"

using namespace std;

class Scene{
private:
	list<Object3D*> objects;
	list<Object3D*> addedObjects;
	list<Object3D*> removedObjects;
	Camera * camera;
	Light * ambientLight;
	list<DirectionalLight*> directionalLights;
	list<PointLight*> pointLights;
	GLuint pointLightsUBO;
	GLuint directionalLightsUBO;
	GLuint ambientLightUBO;
	OctreeNode* octree;
public:
	Scene();
	list<Object3D*> getObjects();
	list<Object3D*> getAddedObjects();
	list<Object3D*> getRemovedObjects();
	void addObject(Object3D* o);
	void removeObject(Object3D* o);
	Camera* getCamera();
	void setCamera(Camera* camera);
	Light* getAmbientLight();
	void setAmbientLight(Light* ambientLight);
	list<DirectionalLight*> getDirectionalLights();
	void addDirectionalLight(DirectionalLight* light);
	list<PointLight*> getPointLights();
	void addPointLight(PointLight* pointLight);
	GLuint getPointLightsUBO();
	void setPointLightsUBO(GLuint pointLightsUBO);
	GLuint getDirectionalLightsUBO();
	GLuint getAmbientLightUBO();
	void setDirectionalLightsUBO(GLuint directionalLightsUBO);
	void setAmbientLightUBO(GLuint ambientLightUBO);
	OctreeNode* getOctree();
	void generateOctree();
	~Scene();
};
#endif
