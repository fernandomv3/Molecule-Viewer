#ifndef LIGHT_H
#define LIGHT_H

#include "object/Object3D.h"
#include "math/Color.h"

class Light : public Object3D {
private:
	Color * color;
public:
	Light();
	virtual ~Light();
	Color* getColor();
	void setColor(Color* color);
};

#endif