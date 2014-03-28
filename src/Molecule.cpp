#include "Molecule.h"
#include "AtomMaterialPool.h"
#include "AtomRadiusTable.h"
#include "object/Mesh.h"
#include "material/PhongMaterial.h"
#include "material/GouraudMaterial.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>
using namespace std;

char* Molecule::substr(const char* source, int i, int n){
	char* substr = new char[n+1];
	int len = strlen(source);
	if(i > len) i= 0;
	strncpy(substr,source + i,n);
	substr[n]='\0';
	return substr;
}

Molecule::Molecule(const char* filename){
	this->numAtoms =0;
	this->x=0;
	this->y=0;
	this->z=0;
	this->readPDB(filename);
}

Molecule::~Molecule(){
	if(this->connections != NULL){
		for(int i =0; i < this->numAtoms; i++){
			delete (this->connections[i]);
		}
		delete this->connections;
	}
}

void Molecule::readPDB(const char* filename){
	ifstream pdbFile;
	char line [90];
	int num = this->numAtoms;
	pdbFile.open(filename);
	if (pdbFile.is_open()){
		this->numAtoms =0;
		AtomMaterialPool* matPool = AtomMaterialPool::getInstance();
		AtomRadiusTable* radiusTable = AtomRadiusTable::getInstance();
		Geometry* atomGeometry = new Geometry();
	    atomGeometry->loadDataFromFile("highres-icosphere.mesh");
	    while (!pdbFile.eof()){
	      pdbFile.getline(line,81);
	      if(strlen(line) == 80){
	      	char* recordName = substr(line,0,6);
	      	if(!strcmp(recordName,"ATOM  ") || !strcmp(recordName,"HETATM")){
	      		char* element = isspace(line[12]) || isdigit(line[12])? substr(line,13,1): substr(line,12,2);
	      		//create material for both representations
	      		Material* atomMaterial = matPool->getAtomMaterial(element);
	      		//create mesh for ball & stick
	      		Mesh* atomMesh = new Mesh(atomGeometry,atomMaterial);
	      		//create mesh for spacefill
	      		Mesh* spacefillMesh = new Mesh(atomGeometry,atomMaterial);
	      		//spacefill is initially invisible
	      		spacefillMesh->setVisible(false);
	      		//get 3D position
	      		char* x = substr(line,30,8);
	      		char* y = substr(line,38,8);
	      		char* z = substr(line,46,8);
	      		//set position for ball & stick
	      		atomMesh->getPosition()->setX(atof(x));
	      		atomMesh->getPosition()->setY(atof(y));
	      		atomMesh->getPosition()->setZ(atof(z));
	      		//set position for spacefill
	      		spacefillMesh->getPosition()->setX(atof(x));
	      		spacefillMesh->getPosition()->setY(atof(y));
	      		spacefillMesh->getPosition()->setZ(atof(z));
	      		//retrieve spacefill radius
	      		float radius = radiusTable->getRadius(element);
	      		//ball & stick has constant size 0.5A
	      		atomMesh->getScale()->setX(0.5);
	      		atomMesh->getScale()->setY(0.5);
	      		atomMesh->getScale()->setZ(0.5);
	      		//set spacefill radius
	      		spacefillMesh->getScale()->setX(radius);
	      		spacefillMesh->getScale()->setY(radius);
	      		spacefillMesh->getScale()->setZ(radius);
	      		
	      		delete y;
	      		delete x;
	      		delete z;
	      		// create both atom objects
	      		Atom* atom = new Atom(element,atomMesh);
	      		Atom* spacefillAtom = new Atom(element,spacefillMesh);
	      		delete element;
	      		//push atoms to lists
	      		this->atoms.push_back(atom);
	      		this->spacefill.push_back(spacefillAtom);
	      		//calculate atom center
	      		this->x += atom->getMesh()->getPosition()->getX();
	      		this->y += atom->getMesh()->getPosition()->getY();
	      		this->z += atom->getMesh()->getPosition()->getZ();
	      		(this->numAtoms)++;
	      	}
	      	delete recordName;
	      }
	    }
	    pdbFile.close();
	    this->x /= this->numAtoms;
	    this->y /= this->numAtoms;
	    this->z /= this->numAtoms;
	    this->calculateConnections(num);
  	}
}

Mesh* Molecule::createBond(Atom* a1, Atom* a2){
	Mesh* mesh = new Mesh();
	Vec3* upVec = new Vec3();
	upVec->setX(0.0);
	upVec->setZ(1.0);
	upVec->setY(0.0);
	Vec3* destVec = new Vec3();
	destVec->setX(a2->getMesh()->getPosition()->getX()- a1->getMesh()->getPosition()->getX());
	destVec->setY(a2->getMesh()->getPosition()->getY()- a1->getMesh()->getPosition()->getY());
	destVec->setZ(a2->getMesh()->getPosition()->getZ()- a1->getMesh()->getPosition()->getZ());
	destVec->normalize();
	Quaternion* quat = Quaternion::rotationBetweenVectors(upVec, destVec);
	delete upVec;
	delete destVec;
	Vec3* position = Molecule::getBondPos(a1->getMesh()->getPosition(),a2->getMesh()->getPosition());

	mesh->getScale()->setX(0.6);
	mesh->getScale()->setY(0.6);
	mesh->getScale()->setZ(0.2);
	Vec3* oldPos= mesh->getPosition();
	mesh->setPosition(position);
	delete oldPos;
	Quaternion* oldQuat = mesh->getQuaternion();
	mesh->setQuaternion(quat);
	delete oldQuat;
	return mesh;
}

Vec3* Molecule::getBondPos(Vec3* atomPos1, Vec3* atomPos2){
	Vec3* pos = new Vec3();
	pos->setX((atomPos1->getX()+ atomPos2->getX())/2.0);
	pos->setY((atomPos1->getY()+ atomPos2->getY())/2.0);
	pos->setZ((atomPos1->getZ()+ atomPos2->getZ())/2.0);
	return pos;
}

void Molecule::calculateConnections(int num){
	if (connections != NULL){
		for(int i=0; i < num; i++){
			delete (this->connections[i]);
		}
	}
	this->connections = new bool*[this->numAtoms];
	for(int i = 0; i < this->numAtoms;i++){
		this->connections[i] = new bool[this->numAtoms];
	}
	Geometry* geom = new Geometry();
	geom->loadDataFromFile("cylinder.mesh");
	Material* mat = new PhongMaterial();
	mat->getDiffuseColor()->setRGB(0.5,0.5,0.5);
	mat->setShininess(1000); 

	for(int i = 0; i < this->numAtoms; i++){
		for(int j=i+1; j< this->numAtoms;j++){
			if( Molecule::atomsConnected(this->atoms[i],this->atoms[j]) ){
				Mesh * bond = createBond(this->atoms[i],this->atoms[j]);
				bond->setGeometry(geom);
				bond->setMaterial(mat);
				this->bonds.push_back(bond);
				this->connections[i][j] = true;
			}
			this->connections[i][j] = false;
		}
	}
}

vector<Atom*> Molecule::getAtoms(){
	return this->atoms;
}

vector<Atom*> Molecule::getSpacefill(){
	return this->spacefill;
}

vector<Mesh*> Molecule::getBonds(){
	return this->bonds;
}

bool** Molecule::getConnections(){
	return this->connections;
}

int Molecule::getNumAtoms(){
	return this->numAtoms;
}

bool Molecule::atomsConnected(Atom* a1, Atom* a2){
	float distance = a1->getMesh()->getPosition()->distance(a2->getMesh()->getPosition());
	if(strcmp(a1->getSymbol(),"H") && strcmp(a2->getSymbol(),"H")){
		if(distance >= 0.4 && distance <= 1.9) return true;
	}
	else{
		if(distance >= 0.4 && distance <= 1.2) return true;
	}
	return false;
}

void Molecule::addToScene(Scene* scene){
	for (int i =0; i < this->numAtoms;i++){
		scene->addObject((Object3D*)(this->atoms[i]->getMesh()));
	}
	int numBonds = this->bonds.size();
	for (int i=0; i < numBonds;i++){
		scene->addObject((Object3D*)(this->bonds[i]));
	}
	for (int i =0; i < this->numAtoms;i++){
		scene->addObject((Object3D*)(this->spacefill[i]->getMesh()));
	}
}

float Molecule::getX(){
	return this->x;
}

float Molecule::getY(){
	return this->y;
}

float Molecule::getZ(){
	return this->z;
}

void Molecule::toggleSpaceFill(){
	for(int i=0; i< this->numAtoms;i++){
		this->spacefill[i]->getMesh()->setVisible(!this->spacefill[i]->getMesh()->getVisible());
		this->atoms[i]->getMesh()->setVisible(!this->atoms[i]->getMesh()->getVisible());
	}
	int numBonds = this->bonds.size();
	for(int i=0; i<numBonds; i++){
		this->bonds[i]->setVisible(!this->bonds[i]->getVisible());
	}
}

