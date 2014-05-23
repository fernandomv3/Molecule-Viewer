#ifndef TESSMATERIAL_H
#define TESSMATERIAL_H
#include "material/Material.h"
	
class TessMaterial:public Material{
private:
	GLchar* tessControlShaderSource;
	GLchar* tessEvaluationShaderSource;
public:
	TessMaterial();
	TessMaterial(const Material &material);
	void makePrograms(int numDirLights, int numPointLights);
};

#endif