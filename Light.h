#ifndef LIGHT_H
#define LIGHT_H
#include "Color.h"
#include "Vec3.h"

class Light 
{
	public:
		Vec3 Position;
		Color Intensity;
		Light();
		~Light(){}
		Light(Vec3 C, Color I);
} ;

Light::Light() {} ;

Light::Light(Vec3 C, Color I)
{
	Position = C;
	Intensity = I;
} ;

#endif
