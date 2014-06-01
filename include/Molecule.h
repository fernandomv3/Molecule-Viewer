#ifndef MOLECULE_H
#define MOLECULE_H
#include <vector>
#include "Atom.h"
#include "scene/Scene.h"
#include "object/Object3D.h"
using namespace std;

class Molecule : public Object3D{
private:
	vector<Atom*> atoms;
	vector<Atom*> spacefill;
	vector<Mesh*> bonds;
	char ** connections;
	char* substr(const char* source, int i, int n);
	float x;
	float y;
	float z;
	int numAtoms;
	void initConnectionMatrix(int numAtoms,char value);
public:
	Molecule(const char* filename);
	Molecule(const Molecule& molecule);
	~Molecule();
	void readPDB(const char* filename);
	vector<Atom*> getAtoms();
	vector<Atom*> getSpacefill();
	vector<Mesh*> getBonds();
	Mesh* createBond(Atom* a1, Atom* a2, int numLinks);
	static Vec3* getBondPos(Vec3* atomPos1, Vec3* atomPos2);
	char** getConnections();
	void calculateConnections(int num);
	int getNumAtoms();
	static bool atomsConnected(Atom* a1, Atom* a2);
	void addToScene(Scene* scene);
	float getX();
	float getY();
	float getZ();
	void toggleSpaceFill();
};

#endif