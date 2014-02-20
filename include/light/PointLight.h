#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light/Light.h"
#include "scene/Camera.h"

struct pLight{
	GLfloat color[4];
	GLfloat position[4];
	GLfloat intensity;
  GLfloat attenuation;
	GLfloat stride[2];
};

typedef struct pLight* PLight;

class PointLight : public Light{
  private:
  	GLfloat intensity;
    GLfloat attenuation;
  public:
  	PointLight();
  	void setIntensity(GLfloat intensity);
  	GLfloat getIntensity();
    void setAttenuation(GLfloat attenuation);
    GLfloat getAttenuation();
  	~PointLight();
  	PLight getAsStruct(Camera* camera);
};

#endif