#ifndef SPHERICALCOORD_H
#define SPHERICALCOORD_H

#include "math/Vec3.h"

class SphericalCoord{
private:
	float phi;
	float theta;
	float r;
public:
	SphericalCoord();
	SphericalCoord(Vec3* cartesian, Vec3* relOrigin=NULL);
	float getPhi();
	float getTheta();
	float getR();
	void setPhi(float phi);
	void setTheta(float theta);
	void setR(float r);
	void setFromCartesian(Vec3 * position, Vec3* relOrigin = NULL);
	Vec3* getCartesian(Vec3 * relOrigin = NULL);
};


#endif