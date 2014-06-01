#include "AtomMaterialPool.h"
#include "material/PhongMaterial.h"
#include "material/GouraudMaterial.h"
#include "material/TessMaterial.h"
#include "material/CelMaterial.h"
#include <fstream>
#include <cstdio>
#include <iostream>
using namespace std;

AtomMaterialPool* AtomMaterialPool::instance = NULL;

AtomMaterialPool::AtomMaterialPool(){
	fstream colorsFile;
	colorsFile.open("colors.txt");
	if(colorsFile.is_open()){
		char element[4];
		char hexColor[8];
		while(!colorsFile.eof()){
			colorsFile >> element >> hexColor;
			Material* mat= new TessMaterial();
			float color[3];
			AtomMaterialPool::RGBfromHexString(color,hexColor);
			mat->getDiffuseColor()->setRGB(color[0],color[1],color[2]);
			mat->setShininess(100);
			string str(element);
			this->pool[str] = mat;
		}
	}
}

AtomMaterialPool* AtomMaterialPool::getInstance(){
	if (AtomMaterialPool::instance == NULL){
		instance = new AtomMaterialPool();
	}
	return instance;
}

void AtomMaterialPool::RGBfromHexString(float* result , const char* hexColor){
	unsigned int rgb[3];
	sscanf(hexColor,"#%02x%02x%02x",&(rgb[0]),&(rgb[1]),&(rgb[2]));
	result[0] = rgb[0]/255.0;
	result[1] = rgb[1]/255.0;
	result[2] = rgb[2]/255.0;
}

Material* AtomMaterialPool::getAtomMaterial(char* element){
	string str(element);
	return pool[str];
}
