#ifndef ROTATION_H
#define ROTATION_H
#include "Vec3.h"
#include "Model.h"
#include <math.h>

class Rotation : public Transformation
{
	public:
		double Degree;
		Rotation();
		~Rotation(){}
		Rotation(Vec3 C, double A);
} ;

Rotation::Rotation() {} ;

Rotation::Rotation(Vec3 C, double A)
{
	Axis = C;
	Degree = A;
} ;

#endif
