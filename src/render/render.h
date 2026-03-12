#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <vector>
#include "box.h"
#include <pcl/visualization/pcl_visualizer.h>

struct Color
{

	float r, g, b;

	Color(float setR, float setG, float setB): r(setR), g(setG), b(setB){}

};

struct Vect3
{

	double x, y, z;

	Vect3(double setX, double setY, double setZ): x(setX), y(setY), z(setZ){}
	
	Vect3 operator+(const Vect3& vec)
	{
		return Vect3(x + vec.x, y + vec.y, z + vec.z);
	}
};

// Forward declaration
class UKF;

struct Car
{
	// Basic rendering
	Vect3 position, dimensions;
	std::string name;
	Color color;
	
	// UKF and movement - added for highway simulation
	double velocity;
	double angle;
	void* ukf;  // UKF object pointer - will be cast to UKF*
	
	bool checkCollision(const Vect3& point) const {
		return (position.x - dimensions.x/2 <= point.x && point.x <= position.x + dimensions.x/2 &&
		        position.y - dimensions.y/2 <= point.y && point.y <= position.y + dimensions.y/2 &&
		        position.z - dimensions.z/2 <= point.z && point.z <= position.z + dimensions.z/2);
	}
	
	// Constructor - basic rendering only
	Car(Vect3 setPosition, Vect3 setDimensions, Color setColor, std::string setName)
    	: position(setPosition), dimensions(setDimensions), color(setColor), name(setName), velocity(0), angle(0), ukf(nullptr)
  	{}

  	void render(pcl::visualization::PCLVisualizer::Ptr& viewer)
	{
		viewer->addCube(position.x-dimensions.x/2, position.x+dimensions.x/2, position.y-dimensions.y/2, position.y+dimensions.y/2, position.z, position.z+dimensions.z, color.r, color.g, color.b, name); 
      	viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_SURFACE, name); 
	    viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, color.r, color.g, color.b, name);
	    viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 1.0, name);
	}
};

bool inbetween(double point, double center, double range)
{
	return (center-range <= point) && (center+range >= point);
}

enum CameraAngle { XY, TopDown, Side, FPS };

void initCamera(CameraAngle setAngle, pcl::visualization::PCLVisualizer::Ptr& viewer)
{

	viewer->setBackgroundColor (0, 0, 0);
	
	viewer->initCameraParameters ();
	// -1.77157, -1.82227, 2.21424
	if (setAngle == XY) {
		viewer->setCameraPosition(0, 0, 16, 1, 1, 0);
	}
	else if (setAngle == TopDown) {
		viewer->setCameraPosition(0, 0, 50, 1, 0, 0);
	}
	else if (setAngle == Side) {
		viewer->setCameraPosition(15, 0, 0, 0, 0, 1);
	}
	else if (setAngle == FPS) {
		viewer->setCameraPosition(0, 0, 5, 0, 0, 1);
	}

	if (setAngle != FPS)
		viewer->addCoordinateSystem (1.0);
}

void renderHighway(pcl::visualization::PCLVisualizer::Ptr& viewer)
{
	viewer->addLine(pcl::PointXYZ(-100, -4, 0), pcl::PointXYZ(100, -4, 0), 1, 1, 1, "line1");
	viewer->addLine(pcl::PointXYZ(-100, 4, 0), pcl::PointXYZ(100, 4, 0), 1, 1, 1, "line2");
}

#endif
