#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Vec3.h"

class Triangle
{
	public:
		vector<Vec3*>  Vertices; 
		Vec3* Normal; 
		Vec3 tempNormal1, tempNormal2; 
		Triangle();
		~Triangle();
		Triangle(Vec3 *vec1,Vec3 *vec2,Vec3 *vec3);
		double IsIntersect(Vec3 vector, Vec3 center);
		double gama,beta,alpha;

} ;

Triangle::Triangle() {};

Triangle::~Triangle() {};

Triangle::Triangle(Vec3 *vec1,Vec3 *vec2,Vec3 *vec3)
{
	Vertices.push_back(vec1);
	Vertices.push_back(vec2);
	Vertices.push_back(vec3);
	Normal = new Vec3() ;
	
} ;

double Triangle::IsIntersect(Vec3 vector, Vec3 center)
{

	tempNormal1 = *(Vertices[1]) - *(Vertices[0]) ;
	tempNormal2 = *(Vertices[2]) - *(Vertices[0]) ;
	*Normal = (tempNormal1.cross(tempNormal2)).normalize();
	
	Vec3 tempDet = vector.cross(tempNormal2);
	double det = tempNormal1.dot(tempDet);

	if(det > (-1.0)*EPSILON && det < EPSILON)
	{
		return 0;
	}
	
	double inv_det = 1.f/det;
	Vec3 distance = center - *(Vertices[0]);
	double test_bound = (distance.dot(tempDet))*inv_det;
	if(test_bound < 0.f - EPSILON  || test_bound > 1.f + EPSILON) return 0;
	Vec3 test_parameter = distance.cross(tempNormal1);
	double test_bound2 = (vector.dot(test_parameter))*inv_det;
	if(test_bound2 < 0.f - EPSILON || test_bound+test_bound2 > 1.f + EPSILON) return 0;
	
	double intersection = (tempNormal2.dot(test_parameter))*inv_det;
	if(intersection > EPSILON) {
		beta = test_bound ;
		gama = test_bound2 ;
		alpha = 1 - beta - gama;
		return intersection;
	}

	return 0;
}

#endif