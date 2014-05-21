#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H
#include "material/Material.h"
	
class PhongMaterial:public Material{
public:
	PhongMaterial();
	void makePrograms(int numDirLights, int numPointLights);
};

#endif