#ifndef ATOM_H
#define ATOM_H
#include "object/Mesh.h"

class Atom{
private:
	char* symbol;
	Mesh* mesh;
public:
	Atom(const char* symbol, Mesh * mesh);
	~Atom();
    char* getSymbol();
    void setSymbol(const char* symbol);
    Mesh* getMesh();
    void setMesh(Mesh* mesh);
};

#endif