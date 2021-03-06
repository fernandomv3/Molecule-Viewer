#ifndef MESH_H
#define MESH_H

#include "object/Geometry.h"
#include "object/Object3D.h"
#include "material/Material.h"

class Mesh : public Object3D{
private:
	Geometry * geometry;
	Material * material;
	BoundingBox boundingBox;
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(Geometry* geometry);
	Mesh(Geometry* geometry, Material* material);
	Material * getMaterial();
	void setMaterial(Material* material);
	Geometry * getGeometry();
	void setGeometry(Geometry* geometry);
	~Mesh();
	BoundingBox getBoundingBox();
	void updateBoundingBox();
};

#endif
