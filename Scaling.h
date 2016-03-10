#ifndef SCALING_H
#define SCALING_H
#include "Vec3.h"
#include "Model.h"

class Scaling : public Transformation
{
	public:
		Scaling();
		~Scaling(){}
		Scaling(Vec3 C);
} ;

Scaling::Scaling() {} ;

Scaling::Scaling(Vec3 C)
{
	Axis = C;
} ;

#endif
