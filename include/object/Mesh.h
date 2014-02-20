#ifndef MESH_H
#define MESH_H

#include "object/Geometry.h"
#include "object/Object3D.h"
#include "material/Material.h"

class Mesh : public Object3D{
private:
	Geometry * geometry;
	Material * material;	
public:
	Mesh();
	Mesh(Geometry* geometry);
	Mesh(Geometry* geometry, Material* material);
	Material * getMaterial();
	void setMaterial(Material* material);
	Geometry * getGeometry();
	void setGeometry(Geometry* geometry);
	~Mesh();
};

#endif
