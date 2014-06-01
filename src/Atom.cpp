#include "Atom.h"

Atom::Atom(const char* symbol, Mesh * mesh){
	this->symbol = strdup(symbol);
	this->mesh = mesh;
}

Atom::Atom(const Atom& atom){
	this->symbol = atom.symbol;
	this->mesh = new Mesh(*(atom.mesh));
}

Atom::~Atom(){
	delete this->symbol;
}

char* Atom::getSymbol(){
	return this->symbol;
}

void Atom::setSymbol(const char* symbol){
	delete symbol;
	this->symbol = strdup(symbol);
}

Mesh* Atom::getMesh(){
	return this->mesh;
}

void Atom::setMesh(Mesh* mesh){
	this->mesh = mesh;
}
