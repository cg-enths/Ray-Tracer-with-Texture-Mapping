#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Vec3.h"

using namespace std;

class Camera
{
	public:	
		int ID;
		Vec3 Position;
		Vec3 Gaze;
		Vec3 Up;
		double ImagePlane[7];
		string OutputName;
		Camera();
		~Camera();
		Camera(int id, Vec3 position, Vec3 gaze, Vec3 up, double imagePlane[7], string outputName);
		void printCamera();
};

Camera::Camera() {} ;

Camera::~Camera() {} ;

Camera::Camera(int id, Vec3 position, Vec3 gaze, Vec3 up, double imagePlane[7], string outputName)
{
	ID = id;
	Position = position ;				
	Gaze = gaze ;				
	Up = up;
	OutputName = outputName;
	
	for(int i = 0 ; i < 7 ; i++)
	{				
		ImagePlane[i] = imagePlane[i] ;
	}
} ;

#endif
