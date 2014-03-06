#include "AtomRadiusTable.h"
#include <fstream>
#include <iostream>
using namespace std;

AtomRadiusTable* AtomRadiusTable::instance = NULL;

AtomRadiusTable::AtomRadiusTable(){
	fstream radiusFile;
	radiusFile.open("radius.txt");
	if(radiusFile.is_open()){
		while(!radiusFile.eof()){
		    string element;
		    float radius; 
			radiusFile >> element >> radius;
			this->table[element] = radius / 100.0;;
		}
	}
}

AtomRadiusTable* AtomRadiusTable::getInstance(){
	if (AtomRadiusTable::instance == NULL){
		instance = new AtomRadiusTable();
	}
	return instance;

}

float AtomRadiusTable::getRadius(char* element){
	string str(element);
	return table[str];
}
