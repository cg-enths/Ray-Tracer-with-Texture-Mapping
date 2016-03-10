#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "Vec3.h"
#include "Camera.h"
#include "Scene.h"
#include "Color.h"
#include "Light.h"
#include "Material.h"
#include "Sphere.h"
#include "Cube.h"
#include "Model.h"
#include "Rotation.h"
#include "Translation.h"
#include "Scaling.h"
#include "Transformation.h"
#include "writePPM.cpp"
#include "aoa_jpeg.c"
#include "aoa_jpeg.h"

using namespace std;

Scene scene;

vector<UCOLOR**> textures;
vector<int*> textureSizes;

Color Interpolation(double x, double y, UCOLOR** image)
{
	int sX = int(x) ; int sY = int(y);
	
	int R = image[sX][sY][0]*(1 - abs(x - sX))*(1 - abs(y - sY))
			+ image[sX + 1][sY][0]*(abs(x - sX))*(1 - abs(y - sY))
			+ image[sX][sY + 1][0]*(1 - abs(x - sX))*(abs(y - sY))
			+ image[sX + 1][sY + 1][0]*(abs(x - sX))*(abs(y - sY)) ;
	
	int G = image[sX][sY][1]*(1 - abs(x - sX))*(1 - abs(y - sY))
			+ image[sX + 1][sY][1]*(abs(x - sX))*(1 - abs(y - sY))
			+ image[sX][sY + 1][1]*(1 - abs(x - sX))*(abs(y - sY))
			+ image[sX + 1][sY + 1][1]*(abs(x - sX))*(abs(y - sY)) ;
			
	int B = image[sX][sY][2]*(1 - abs(x - sX))*(1 - abs(y - sY))
			+ image[sX + 1][sY][2]*(abs(x - sX))*(1 - abs(y - sY))
			+ image[sX][sY + 1][2]*(1 - abs(x - sX))*(abs(y - sY))
			+ image[sX + 1][sY + 1][2]*(abs(x - sX))*(abs(y - sY)) ;

	return Color(R,G,B);
}


Vec3 ComputeEPSILON(Vec3 ray)
{
	double a,b,c ;
	a = ray.x < 0 ? -1 : 1;
	b = ray.y < 0 ? -1 : 1;
	c = ray.z < 0 ? -1 : 1;
	return Vec3(a,b,c)*EPSILON;
}

Vec3 ComputeRay(double posX,double posY, Camera camera)
{
	double toX = camera.ImagePlane[0] + ((camera.ImagePlane[1] - camera.ImagePlane[0])*(posX + 0.5))/camera.ImagePlane[5];
	double toY = camera.ImagePlane[2] + ((camera.ImagePlane[3] - camera.ImagePlane[2])*(posY + 0.5))/camera.ImagePlane[6];
	double toZ = camera.ImagePlane[4];
	Vec3 ray = ((camera.Up).cross(camera.Gaze)*toX) + (camera.Up)*toY + (camera.Gaze)*toZ ;
	return ray.normalize();
}

Color ComputePixelColor(Vec3 ray, Vec3 center, int reflectionCount)
{	
	Model* tempModel;
	Color resultColor = Color(0,0,0);
	double intersection = MAX_DOUBLE;
	unsigned int index;
	Vec3 intersectionPoint; 
	Color intensityEffect;
	for(index = 0 ; index < scene.Models.size() ; index++)
	{
		double tempIntersection = (scene.Models[index])->IsIntersect(ray,center); 
		if(tempIntersection > 0 && tempIntersection<intersection - EPSILON && tempIntersection < MAX_DOUBLE)
		{
			intersection = tempIntersection;
			tempModel = (scene.Models[index]);	
		}
	}

	if(intersection != MAX_DOUBLE)
	{
		intersectionPoint = center + ray*intersection;
		for(unsigned int indexL = 0 ; indexL<scene.Lights.size() ; indexL++)
		{
			Vec3 lightDirection = (scene.Lights[indexL].Position - intersectionPoint).normalize();
			bool isShadow = false;
			double lightBlockCalc = MAX_DOUBLE;
			
			for(index = 0 ; index<scene.Models.size() ; index++)
			{
				double tempLightBlockCalc = (scene.Models[index])->IsIntersect(lightDirection, intersectionPoint); 
				if(tempLightBlockCalc > EPSILON && (intersectionPoint + lightDirection*tempLightBlockCalc).length(intersectionPoint) < intersectionPoint.length(scene.Lights[indexL].Position))
				{
					isShadow = true;
					break;
				}
			}

			if(!isShadow)
			{
				double cosDiffuse = ((tempModel -> getNormal(intersectionPoint)).dot(lightDirection));
				Vec3 halfVector = (lightDirection - ray)/((lightDirection - ray).length(Vec3(0,0,0)));
				double cosSpecular = ((tempModel -> getNormal(intersectionPoint)).dot(halfVector));
				intensityEffect = (scene.Lights[indexL].Intensity);

				if(tempModel->TextureId != 0)
				{
					double *uv = tempModel -> getTexture(intersectionPoint);
					Color tempColor = ((uv[1] < (tempModel -> textureSizes)[0] - 1) && (uv[0] < (tempModel -> textureSizes)[1] - 1)) == 1 ? (Interpolation(uv[1],uv[0],textures[tempModel->TextureId - 1])) :
									Color(int((textures[tempModel -> TextureId - 1])[int(uv[1])][int(uv[0])][0]),int((textures[tempModel -> TextureId - 1])[int(uv[1])][int(uv[0])][1]), int((textures[tempModel -> TextureId - 1])[int(uv[1])][int(uv[0])][2]));
					resultColor = resultColor + tempColor*cosDiffuse
								+ (intensityEffect*(tempModel -> ModelMaterial).Specular)*pow(cosSpecular,(tempModel -> ModelMaterial).SpecExp);
				}
				else
				{
					resultColor = resultColor + ((intensityEffect*(tempModel -> ModelMaterial).Diffuse)*cosDiffuse 
								+ (intensityEffect*(tempModel -> ModelMaterial).Specular)*pow(cosSpecular,(tempModel -> ModelMaterial).SpecExp));	
				}

			}
		}
	}

	if(intersection == MAX_DOUBLE)
	{		
		if(reflectionCount == scene.ReflectionCount)
		{
			resultColor = scene.Background;
		}
	}
	else
	{
		if(reflectionCount != 0)
		{
			Vec3 reflectedRay = ray - (tempModel -> getNormal(intersectionPoint))*(ray.dot((tempModel -> getNormal(intersectionPoint))))*2;
			if(!((tempModel -> ModelMaterial).Reflectance == Color(0,0,0)))
				resultColor = resultColor + ComputePixelColor(reflectedRay.normalize(), intersectionPoint + ComputeEPSILON(reflectedRay), reflectionCount - 1)*(tempModel -> ModelMaterial).Reflectance ;
		}
		if(reflectionCount == scene.ReflectionCount)
		{
			resultColor = resultColor + (scene.Ambient*(tempModel -> ModelMaterial).Ambient);
		}
	}
	
	return resultColor;
	

}
int main(int argc,char *argv[])
{ 
	std::ifstream sceneFile(argv[1]);
	std::ifstream cameraFile(argv[2]);

	string dummy;
	//---------------------------------------------------------------------------------------------------------SCENE PARSING--------------------------------------------------------------------------------------------------------------------//
	
	sceneFile >> scene.ReflectionCount;
	sceneFile >> scene.Background.R >> scene.Background.G >> scene.Background.B ;
	sceneFile >> scene.Ambient.R >> scene.Ambient.G >> scene.Ambient.B ;
	//------------------------------------------------LIGHT SOURCES---------------------------------------------//

	int lightCount = 0;
	sceneFile >> lightCount;
	
	vector<Light> lights;
	for(int i = 0 ; i < lightCount ; i++ )
	{
		double R,G,B;
		double x,y,z;
		sceneFile >> x >> y >> z;
		sceneFile >> R >> G >> B;
		Color color(R,G,B);
		Vec3 vec3(x,y,z);
		Light tempLight(vec3,color);
		lights.push_back(tempLight);		
	}
	scene.Lights = lights;
	
	//------------------------------------------------MATERIALS---------------------------------------------//

	int materialCount = 0;
	sceneFile >> materialCount;
	vector<Material> materials;
	
	for(int i = 0 ; i < materialCount ; i++ )
	{
		int id;
		sceneFile >> dummy >> id ;
		double R,G,B;
		sceneFile >> R >> G >> B;
		Color colorAmbient(R,G,B);
		sceneFile >> R >> G >> B;
		Color colorDiffuse(R,G,B);
		sceneFile >> R >> G >> B;
		Color colorSpec(R,G,B);
		double sxp;
		sceneFile >> sxp;
		sceneFile >> R >> G >> B;
		Color colorReflectance(R, G, B);
		
		Material tempMaterial(id, colorAmbient, colorDiffuse, colorSpec, sxp, colorReflectance);
		materials.push_back(tempMaterial);	
	}
	scene.Materials = materials;
	
	//------------------------------------------------TEXTURES--------------------------------------------//
	sceneFile >> dummy ;
	
	int textureCount = 0;
	sceneFile >> textureCount;
	
	string textureName;
	for(int i = 0 ; i < textureCount ; i++ )
	{
		sceneFile >> textureName; 
		char* Tname = new char(textureName.length() + 1);
		for(int k = 0 ; k < textureName.length() ; k++) Tname[k] = textureName[k]; 
		Tname[textureName.length()] = '\0'; 
		
		int *x = new int[1];
		int *y = new int[1];
		read_jpeg_header(Tname, x, y);
		
		UCOLOR** new_image ;

		new_image = new UCOLOR*[*y];
		for(int t = 0 ; t < *y ; t++)
			new_image[t] = new UCOLOR[*x];

		read_jpeg(Tname, new_image, x, y);
		
		int *sizes = new int[2];
		sizes[1] = *x; sizes[0] = *y;
		textureSizes.push_back(sizes);
		textures.push_back(new_image);
	}

	//------------------------------------------------TRANSITIONS---------------------------------------------//
	sceneFile >> dummy ;

	int translationCount = 0;
	sceneFile >> translationCount;
	
	for(int i = 0 ; i < translationCount ; i++ )
	{
		double x,y,z;
		sceneFile >> x >> y >> z; 
		scene.Translations.push_back(Translation(Vec3(x,y,z)));		
	}

	//------------------------------------------------SCALINGS---------------------------------------------//
	sceneFile >> dummy ;

	int scalingCount = 0;
	sceneFile >> scalingCount;
	
	for(int i = 0 ; i < scalingCount ; i++ )
	{
		double x,y,z;
		sceneFile >> x >> y >> z; 
		scene.Scalings.push_back(Scaling(Vec3(x,y,z)));		
	}

	//------------------------------------------------ROTATIONS---------------------------------------------//
	sceneFile >> dummy ;

	int rotationCount = 0;
	sceneFile >> rotationCount;
	for(int i = 0 ; i < rotationCount ; i++ )
	{
		double a,x,y,z;
		sceneFile >> a >> x >> y >> z;
		scene.Rotations.push_back(Rotation(Vec3(x,y,z), a));		
	}

	//------------------------------------------------MODELS---------------------------------------------//
		
	int modelCount = 0;
	sceneFile >> modelCount;
	vector<Model*> models;
	Cube *tempCube;
	Sphere *tempSphere;
	for(int i = 0 ; i < modelCount ; i++ )
	{
		string type;
		int materialId, textureId, transformationCount;
		sceneFile >> type >> materialId >> textureId >> transformationCount ;
		Material tempMaterial = scene.Materials.at(materialId-1);
		vector<Transformation> transformations;
		Model *tempModel;
		if(type=="#CubeInstance")
		{
			tempCube = new Cube(tempMaterial, textureId);
		 	tempModel = tempCube;
		}
		
		else if(type=="#SphereInstance")
		{
			tempSphere = new Sphere(tempMaterial, textureId);
		 	tempModel = tempSphere;
		}	
		if(textureId != 0)
			tempModel->textureSizes = textureSizes[textureId-1];
		
		for(int j = 0 ; j < transformationCount ; j++)
		{
			char type2; int id;
			sceneFile >> type2 >> id;
			switch(type2)
			{
				case 's':
					tempModel->Scale(scene.Scalings.at(id-1).Axis);
					break;
				case 't':
					tempModel->Translate(scene.Translations.at(id-1).Axis);
					break;
				case 'r':
					tempModel->Rotate(scene.Rotations.at(id-1).Axis,scene.Rotations.at(id-1).Degree);	
					break;
			}
		}
		models.push_back(tempModel);
		

	}
	
	scene.Models = models;
	
	//---------------------------------------------------------------------------------------------------------CAMERA PARSING--------------------------------------------------------------------------------------------------------------------//

	int cameraCount = 0;
	
	cameraFile >> cameraCount;
	
	Camera* cameras = new Camera[cameraCount];
	
	for(int i = 0 ; i < cameraCount ; i++ )
	{
		int tempID; 
		string tempOutputName, objectName;
		Vec3 tempPosition, tempGaze, tempUp;
		double tempImagePlane[7];

		cameraFile >> objectName >> tempID ; 
		cameraFile >> tempPosition.x >> tempPosition.y >> tempPosition.z ;
		cameraFile >> tempGaze.x >> tempGaze.y >> tempGaze.z ;
		cameraFile >> tempUp.x >> tempUp.y >> tempUp.z ;
		cameraFile >> tempImagePlane[0] >> tempImagePlane[1] >> tempImagePlane[2] >> tempImagePlane[3] >> tempImagePlane[4] >> tempImagePlane[5] >> tempImagePlane[6]; 
		cameraFile >> tempOutputName ;

		Camera tempCamera(tempID, tempPosition, tempGaze, tempUp, tempImagePlane, tempOutputName);
		cameras[i] = tempCamera;
	}
	
	//---------------------------------------------------------------------------------------------------------ALGORITHM--------------------------------------------------------------------------------------------------------------------//
	
	for(int cam = 0 ; cam < cameraCount ; cam ++)
	{		
		int height = cameras[cam].ImagePlane[5];
		int width = cameras[cam].ImagePlane[6];
		int size = height*width*3;
		double *data = new double[size];
		
		Color resultColor;

		for(int i = 0 ; i < width ; i++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				
				Vec3 ray = ComputeRay(j, i, cameras[cam]);
				resultColor = ComputePixelColor(ray, cameras[cam].Position + ComputeEPSILON(ray), scene.ReflectionCount);

				data[size--] = resultColor.R;
				data[size--] = resultColor.B;
				data[size--] = resultColor.G;
			}
		}

		writePPM(cameras[cam].OutputName.c_str(), height, width, data);
	}
	return 0;
}
