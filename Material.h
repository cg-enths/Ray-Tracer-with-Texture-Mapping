#ifndef MATERIAL_H
#define MATERIAL_H
#include "Color.h"

class Material
{
	public:
		int ID;
		int SpecExp;
		Color Ambient;
		Color Diffuse;
		Color Specular;
		Color Reflectance;
		Material();
		~Material();
		Material(int id, Color amb, Color diff, Color spec, double sxp, Color ref);
};

Material::Material() {};

Material::~Material() {};

Material::Material(int id, Color amb, Color diff, Color spec, double sxp, Color ref)
{
	ID = id;
	Ambient = amb;
	Diffuse = diff;
	Specular = spec;
	SpecExp = sxp;
	Reflectance = ref;
} ;
#endif
