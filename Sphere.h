#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"
#include "Model.h"
#include "Material.h" 

class Sphere : public Model 
{
	public:
		Sphere();
		~Sphere();
		double R ;
		Sphere(Material material, int texture);
		double IsIntersect(Vec3 vector, Vec3 center);
		Vec3 getNormal(Vec3 point);
		void Transform(Transformation T);
		void Scale(Vec3 vec3);
		void Translate(Vec3 vec3);
		void Rotate(Vec3 vec3,double degree);
		vector<double> degrees;
		vector<Vec3> axiss;
		double* getTexture(Vec3 point);
} ;

Sphere::Sphere() 
{
	this->R =1.0;
	this->Center = Vec3(0,0,0);	
};
Sphere::~Sphere() {};

Sphere::Sphere(Material material, int texture)
{
	this->R =  1.0;
	this->Center = Vec3(0,0,0);
	this->ModelMaterial = material;
	this->TextureId = texture;
} ;

double Sphere::IsIntersect(Vec3 vector, Vec3 center)
{
	double bottom = vector.dot(vector);
	Vec3 temp = center - Center;
	double up_left = (-1)*(vector.dot(temp)) ;
	double up_right = sqrt(up_left*up_left - bottom*(temp.dot(temp) - R*R)) ;
	if(up_right >= 0)
	{
		double res1 = (up_left + up_right)/bottom ;
		double res2 = (up_left - up_right)/bottom ;
		return (res1 > 0 && res2 > 0) ? fmin(res1,res2) : ((res1 > 0 || res2 > 0) ? fmax(res1,res2) : 0); 
	}
	
	return 0 ;

}

Vec3 Sphere::getNormal(Vec3 point)
{
	return (point-Center).normalize();
}
void Sphere::Scale(Vec3 vec3)
{
	this->R = this->R * vec3.x;
}
Vec3 rotateX(Vec3 vec3, double sinus, double cosinus)
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
Vec3 rotateY(Vec3 vec3, double sinus, double cosinus)
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

void Sphere::Rotate(Vec3 vec3, double degree)
{
	Vec3 c=this->Center;
	if(c == Vec3(0,0,0) || c.normalize() == vec3.normalize())
	{
		this-> degrees.push_back(degree);
		this-> axiss.push_back(vec3);
		return ;
	}
	double x = vec3.x;
	double y = vec3.y;
	double z = vec3.z;
	int a=0;
	if(y !=0 )
	{
		c=rotateX(c,y/sqrt(y*y+z*z),z/sqrt(y*y+z*z));
		c=rotateY(c,sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
	}
	else if(z != 0)
	{	
		c=rotateY(c,sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
		a=1;
	}
	double sinus = sin((degree*PI)/180);
	double cosinus = cos((degree*PI)/180);
	c=rotateX(c,sinus,cosinus);
	if(a == 1 )
		c=rotateY(c,-sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
	else if(y !=0 )
	{
		c=rotateY(c,-sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
		c=rotateX(c,-y/sqrt(y*y+z*z),z/sqrt(y*y+z*z));
	}

	
}
Vec3 rotate2(Vec3 Center, Vec3 vec3, double degree)
{
	Vec3 c=Center;
	
	double x = vec3.x;
	double y = vec3.y;
	double z = vec3.z;
	int a=0;
	if(y !=0 )
	{
		c=rotateX(c,y/sqrt(y*y+z*z),z/sqrt(y*y+z*z));
		c=rotateY(c,sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
	}
	else if(z != 0)
	{	
		c=rotateY(c,sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
		a=1;
	}
	double sinus = sin((degree*PI)/180);
	double cosinus = cos((degree*PI)/180);
	c=rotateX(c,sinus,cosinus);
	if(a == 1 )
		c=rotateY(c,-sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
	else if(y !=0 )
	{
		c=rotateY(c,-sqrt(y*y+z*z)/sqrt(y*y+z*z+x*x),x/sqrt(y*y+z*z+x*x));
		c=rotateX(c,-y/sqrt(y*y+z*z),z/sqrt(y*y+z*z));
	}
	return c;

	
}
void Sphere::Translate(Vec3 vec3)
{
	this->Center = this->Center + vec3;
}

double* Sphere::getTexture(Vec3 point)
{
	double* uv = new double[2];
	
	for(int i=axiss.size()-1 ; i>=0 ; i--)
	{	
		point = this->Center+(rotate2((point - this->Center)/this->R ,axiss[i],-degrees[i]))*(this->R);
	}
	
	double tempTeta = ((point.y - (this->Center).y))/R;
	double teta = acos(tempTeta);
	double gama = atan2((point.z - (this->Center).z),(point.x - (this->Center).x));
	
	uv[0] = (int)((0.5 - gama/(2.0*PI))*textureSizes[1])% (int)textureSizes[1];
	uv[1] = (teta/PI)*textureSizes[0];

	return uv;
}

#endif
