#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "object/Object3D.h"
#include "light/Light.h"
#include "scene/Camera.h"

struct dirLight{
	GLfloat color[4];
	GLfloat vectorToLight[4];
	GLfloat intensity;
	GLfloat extra[3];
};

typedef struct dirLight* DirLight;

class DirectionalLight : public Light{
private:
	Object3D * target;
	GLfloat intensity;
public:
	DirectionalLight();
	~DirectionalLight();
	Object3D* getTarget();
	GLfloat getIntensity();
	void setIntensity(GLfloat intensity);
	DirLight getAsStruct(Camera* camera);
	GLfloat* getVectorToLightAsArray(Camera* camera);
};

#endif