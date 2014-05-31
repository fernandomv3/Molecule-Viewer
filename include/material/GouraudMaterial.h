#ifndef GOURAUDMATERIAL_H
#define GOURAUDMATERIAL_H
#include "material/Material.h"
	
class GouraudMaterial:public Material{
public:
	GouraudMaterial();
	void makePrograms(Scene* scene);
};

#endif