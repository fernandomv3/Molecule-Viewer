#ifndef CELMATERIAL_H
#define CELMATERIAL_H
#include "material/Material.h"
	
class CelMaterial:public Material{
public:
	CelMaterial();
	void makePrograms(int numDirLights, int numPointLights);
};

#endif