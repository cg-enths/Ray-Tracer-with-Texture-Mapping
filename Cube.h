#ifndef CUBE_H
#define CUBE_H

#include "Vec3.h"
#include "Model.h"
#include "Material.h" 
#include "Transformation.h" 
#include "Triangle.h"

class Cube : public Model 
{
	public:
		Cube();
		~Cube();
		vector <Vec3> edges;
		int triangleId;
		Cube(Material material,int texture);
		double IsIntersect(Vec3 vector, Vec3 center);
		Vec3 getNormal(Vec3 point);
		vector<Triangle> triangles; 
		void Transform(Transformation T);
		void Scale(Vec3 vec3);
		void Translate(Vec3 vec3);
		void Rotate(Vec3 vec3,double degree);
		double *startingPoints(int i);
		Vec3 Normal;
		double* getTexture(Vec3 point);
} ;

Cube::Cube() 
{
	this->Center = Vec3(0,0,0);
};

Cube::~Cube() {};

Cube::Cube(Material material, int texture)
{
	this->Center = Vec3(0,0,0);
	this->ModelMaterial = material;
	this->TextureId = texture;
	this->edges.push_back(Vec3(0.5,0.5,0.5)); //0  		+ + + 
	this->edges.push_back(Vec3(0.5,0.5 ,-0.5)); //1 		+ + -
	this->edges.push_back(Vec3(-0.5 ,0.5,-0.5)); //2 		- + -
	this->edges.push_back(Vec3(-0.5,0.5 ,0.5 )); //3		- + +
	this->edges.push_back(Vec3(-0.5,-0.5,0.5)); //4		- - +
	this->edges.push_back(Vec3(0.5,-0.5,0.5)); //5		+ - +
	this->edges.push_back(Vec3(-0.5,-0.5,-0.5)); //6		- - -
	this->edges.push_back(Vec3(0.5,-0.5,-0.5 )); //7		+ - -
	

	this->triangles.push_back(Triangle(&(this->edges[2]),&(this->edges[6]),&(this->edges[4])));
	this->triangles.push_back(Triangle(&(this->edges[2]),&(this->edges[3]),&(this->edges[4])));
	
	this->triangles.push_back(Triangle(&(this->edges[2]),&(this->edges[6]),&(this->edges[7])));
	this->triangles.push_back(Triangle(&(this->edges[2]),&(this->edges[1]),&(this->edges[7])));

	this->triangles.push_back(Triangle(&(this->edges[6]),&(this->edges[4]),&(this->edges[5])));
	this->triangles.push_back(Triangle(&(this->edges[6]),&(this->edges[7]),&(this->edges[5])));

	this->triangles.push_back(Triangle(&(this->edges[3]),&(this->edges[4]),&(this->edges[5])));
	this->triangles.push_back(Triangle(&(this->edges[3]),&(this->edges[0]),&(this->edges[5])));

	this->triangles.push_back(Triangle(&(this->edges[0]),&(this->edges[5]),&(this->edges[7])));
	this->triangles.push_back(Triangle(&(this->edges[0]),&(this->edges[1]),&(this->edges[7])));
	
	this->triangles.push_back(Triangle(&(this->edges[2]),&(this->edges[3]),&(this->edges[0])));
	this->triangles.push_back(Triangle(&(this->edges[2]),&(this->edges[1]),&(this->edges[0])));	

	triangleId = -1;
} ;

double *Cube::startingPoints(int i)
{
	double *points= new double[2];
	if(i>5)
	{
		points[0] = 0.5*(this->textureSizes[0]);		
	}
	if(i<=5)
	{
		points[0] = 0;
	}
	if(i == 0 || i == 1 || i == 6 || i == 7)
	{
		points[1] = 0;
	}
	if(i == 2 || i == 3 || i == 8 || i == 9)
	{
		points[1] = (this->textureSizes[1])/3.0;
	}
	if(i == 4 || i == 5 || i == 10 || i == 11)
	{
		points[1] = 2.0*(this->textureSizes[1])/3;
	}
	
	return points;
}

double* Cube::getTexture(Vec3 point)
{
	double* uv = new double[2];

	double *sP = startingPoints(this-> triangleId) ;
	
	if(triangleId%2)
	{
		uv[0] = sP[1] + ((this->textureSizes[1])/3)*(this->triangles)[triangleId].gama ;
		uv[1] = sP[0]  + ((this->textureSizes[0])/2)*(this->triangles)[triangleId].beta + ((this->textureSizes[0])/2)*(this->triangles)[triangleId].gama;
	}
	else
	{
		uv[0] = sP[1] + ((this->textureSizes[1])/3)*(this->triangles)[triangleId].beta + ((this->textureSizes[1])/3)*(this->triangles)[triangleId].gama;
		uv[1] = sP[0]  + ((this->textureSizes[0])/2)*(this->triangles)[triangleId].gama;
	}

	uv[0] = (uv[0] < EPSILON) ? sP[1] + 1 : (uv[0] > (this->textureSizes[1]) ? sP[1] + (this->textureSizes[1])/3 - 1 : uv[0]) ;
	uv[1] = (uv[1] < EPSILON) ? sP[0] + 1 : (uv[1] > (this->textureSizes[0]) ? sP[0] + (this->textureSizes[0])/2 - 1 : uv[1]) ;

	return uv;
}

double Cube::IsIntersect(Vec3 vector, Vec3 center)
{

	double intersection = MAX_DOUBLE;

	for(int i = 0 ; i < this->triangles.size(); i++)
	{
		double tempIntersection = triangles[i].IsIntersect(vector,center); 
		if(tempIntersection > EPSILON && tempIntersection < intersection && tempIntersection < MAX_DOUBLE)
		{
			intersection = tempIntersection;
			triangleId = i;
			Normal = i%2 == 1 ? *(triangles[i-1].Normal) : *(triangles[i].Normal) ;
		}
	}
	return intersection ;
}

Vec3 Cube::getNormal(Vec3 point)
{
	if(triangleId == 2 || triangleId == 3)
		return Normal*(-1);
	return Normal;
}

void Cube::Scale(Vec3 vec3)
{
	for(int i = 0 ; i < this->edges.size(); i++)
	{
		(this->edges)[i] = (this->edges)[i]-(this->Center);
		(this->edges)[i] = Vec3(this->edges[i].x * vec3.x,this->edges[i].y * vec3.y,this->edges[i].z * vec3.z);		
		(this->edges)[i] = (this->edges)[i]+(this->Center);

	}
}

Vec3 rotatex(Vec3 vec3, double sinus, double cosinus)
{
	Vec3 result = vec3;
	double x = result.x;
	double y = result.y;
	double z = result.z;
	
	result.x = x;
	result.y = y*cosinus - z*sinus;
	result.z = y*sinus + z*cosinus;

	return result;
}

Vec3 rotatey(Vec3 vec3, double sinus, double cosinus)
{
	Vec3 result = vec3;
	double x = result.x;
	double y = result.y;
	double z = result.z;
	
	result.y = y;
	result.x = x*cosinus + z*sinus;
	result.z = -x*sinus + z*cosinus;

	return result;	
}

Vec3 rotate(Vec3 Center, Vec3 vec3, double degree)
{
	Vec3 c=Center;
	
	double x = vec3.x;
	double y = vec3.y;
	double z = vec3.z;

	int a=0;
	if(y !=0 )
	{
		c=rotatex(c,y/sqrt(y*y+z*z),z/sqrt(y*y+z*z));
		c=rotatey(c,sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
	}
	else if(z != 0)
	{	
		c=rotatey(c,sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
		a=1;
	}
	double sinus = sin((degree*PI)/180);
	double cosinus = cos((degree*PI)/180);
	c=rotatex(c,sinus,cosinus);
	if(a == 1 )
		c=rotatey(c,-sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
	else if(y !=0 )
	{
		c=rotatey(c,-sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
		c=rotatex(c,-y/sqrt(y*y+z*z),z/sqrt(y*y+z*z));
	}
	
	return c;
}

void Cube::Rotate(Vec3 vec3, double degree)
{
	if((this->Center).x !=0 || (this->Center).y !=0 || (this->Center).z !=0)
		this->Center = rotate(this->Center , vec3 , degree);
	for(int i = 0 ; i < this->edges.size(); i++)
	{
		if(degree!=0)
		{
			(this->edges)[i] = rotate((this->edges)[i] , vec3 , degree);
			
		}
	}
	
}

void Cube::Translate(Vec3 vec3)
{
	this->Center=this->Center+vec3;
	for(int i = 0 ; i < this->edges.size(); i++)
	{
		(this->edges)[i] = (this->edges)[i] + vec3;
	}
}

#endif
