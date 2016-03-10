#ifndef MODEL_H
#define MODEL_H
#include "Material.h"
#include "Vec3.h"
#include "Transformation.h"

class Model
{
	public:;
		int TextureId;
		Material ModelMaterial;
		Vec3 Normal;
		Vec3 Center;
		int *textureSizes;
		Model() {} ;
		virtual double IsIntersect(Vec3 vector, Vec3 center) = 0;
		virtual Vec3 getNormal(Vec3 point) = 0;
		virtual void Scale(Vec3 vec3) = 0;
		virtual void Translate(Vec3 vec3) = 0;
		virtual void Rotate(Vec3 vec3,double degree) = 0;
		virtual double* getTexture(Vec3 point) = 0;
};

#endif
