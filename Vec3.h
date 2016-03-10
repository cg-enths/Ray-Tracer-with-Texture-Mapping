#ifndef VEC_H
#define VEC_H

#include <cmath>

#define MAX_DOUBLE 9999999.99
#define PI 3.14159265358979323846
#define EPSILON 0.000001

using namespace std;

class Vec3 {
    
    public:
        double x, y, z;
        Vec3();
        ~Vec3();
        Vec3(double, double, double);
        int operator==(Vec3 V);
        Vec3 operator+(Vec3 V);
        Vec3 operator-(Vec3 V);
        Vec3 operator*(Vec3 V);
        Vec3 operator/(Vec3 V);
        Vec3 operator*(double number);
        Vec3 operator/(double number);
        Vec3 cross(Vec3 V);
        Vec3 normalize();
        double dot(Vec3 V);
        double length(Vec3 V) ;
};

Vec3::Vec3() {} ;
Vec3::~Vec3() {} ;

Vec3::Vec3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
} ;

int Vec3::operator==(Vec3 V) 
{
    return(x == V.x && y == V.y && z == V.z);
} ;

Vec3 Vec3::operator+(Vec3 V) 
{    
    return Vec3(x + V.x, y + V.y, z + V.z) ;
} ;

Vec3 Vec3::operator-(Vec3 V)
{    
    return Vec3(x - V.x, y - V.y, z - V.z) ;
} ;

Vec3 Vec3::operator*(double number)
{    
    return Vec3(x*number, y*number, z*number);
} ;

Vec3 Vec3::operator/(double number) 
{    
    return Vec3(x/number, y/number, z/number);
} ;

Vec3 Vec3::cross(Vec3 V) 
{    
    return Vec3( y*V.z - z*V.y, z*V.x - x*V.z, x*V.y - y*V.x);
} ;

double Vec3::dot(Vec3 V) 
{   
    return (x * V.x + y * V.y + z * V.z);
} ;

double Vec3::length(Vec3 V) 
{
    return double(sqrt(pow(x - V.x,2) + pow(y - V.y,2) + pow(z - V.z,2)));
};

Vec3 Vec3::normalize() 
{
	double length = this->length(Vec3(0,0,0));
    return Vec3(x/length, y/length, z/length);
};
#endif
