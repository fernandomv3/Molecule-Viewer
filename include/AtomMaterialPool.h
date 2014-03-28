#ifndef ATOMMATERIALPOOL_H
#define ATOMMATERIALPOOL_H
#include <map>
#include "material/Material.h"
#include <string>
using namespace std;

class AtomMaterialPool{
private:
	static AtomMaterialPool* instance;
	map<string,Material *> pool;
	static void RGBfromHexString(float* result, const char* hexColor);
	AtomMaterialPool();
public:
	static AtomMaterialPool* getInstance();
    Material* getAtomMaterial(char* element);
};

#endif