#ifndef MESH_H
#define MESH_H

#include "Triangle.h"
#include "Model.h"
#include "Material.h"

class Mesh : public Model
{
	public:
		vector<Triangle> Triangles;
		Mesh();
		~Mesh();
		Mesh(vector<Triangle> T, Material M, int Id);
		double IsIntersect(Vec3 vector, Vec3 center);
		Vec3 getNormal(Vec3 point);
} ;

Mesh::Mesh() {};
Mesh::~Mesh() {};

Mesh::Mesh(vector<Triangle> T, Material M, int Id)
{
	Triangles = T;
	ModelMaterial = M;
	this->Id=Id;
} ;

double Mesh::IsIntersect(Vec3 vector, Vec3 center)
{
	/*Möller–Trumbore_intersection_algorithm*/
	/*https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm*/
	//cramerdeki determinant hesabinin kisa yolu
	double intersection = 0;
	for(int i=0;i<Triangles.size();i++)
	{
		double tempIntersection = Triangles[i].IsIntersect(vector, center);
		if(tempIntersection!=0 && (intersection==0 || tempIntersection<intersection))
		{
			intersection=tempIntersection;
			Normal = Triangles[i].Normal;
		}
	}
	return intersection;

}

Vec3 Mesh::getNormal(Vec3 point)
{
	return this-> Normal;
};
#endif
