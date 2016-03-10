#ifndef TRANSLATION_H
#define TRANSLATION_H
#include "Vec3.h"
#include "Model.h"

class Translation : public Transformation
{
	public:
		Translation();
		~Translation(){}
		Translation(Vec3 C);
} ;

Translation::Translation() {} ;

Translation::Translation(Vec3 C)
{
	Axis = C;
} ;

#endif
