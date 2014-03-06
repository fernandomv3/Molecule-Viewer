#ifndef ATOMRADIUSTABLE_H
#define ATOMRADIUSTABLE_H

#include <map>
#include <string>
using namespace std;

class AtomRadiusTable{
private:
	static AtomRadiusTable * instance;
	map<string,float> table;
	AtomRadiusTable();
public:
	static AtomRadiusTable* getInstance();
	float getRadius(char* element);
};

#endif