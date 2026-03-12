/*
 * UKF Highway - GUI Version
 * Uses PCL for 3D visualization of the Unscented Kalman Filter
 * 
 * This runs the full highway simulation with:
 * - 3D viewer showing ego car (green) and traffic cars (blue)
 * - Lidar detection (red spheres)
 * - Radar detection (purple lines)
 * - UKF tracking predictions
 * - Real-time RMSE display
 */

#include <iostream>
#include "highway.h"

int main(int argc, char** argv)
{
    std::cout << "Starting UKF Highway GUI..." << std::endl;
    std::cout << "Use mouse to rotate/zoom the 3D view" << std::endl;
    std::cout << "Press 'Q' or 'Esc' to exit" << std::endl;
    
    // Create 3D viewer
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("UKF Highway Viewer"));
    
    // Set camera angle - options: XY, TopDown, Side, FPS
    CameraAngle setAngle = XY;
    
    // Initialize camera
    viewer->setBackgroundColor(0, 0, 0);
    viewer->initCameraParameters();
    int distance = 16;
    viewer->setCameraPosition(-distance, -distance, distance, 1, 1, 0);
    viewer->addCoordinateSystem(1.0);
    
    // Create highway environment
    Highway highway(viewer);
    
    // Animation parameters
    double egoVelocity = 25;  // m/s
    int frame_per_sec = 30;
    long long timestamp = 0;
    int frameCount = 0;
    int maxFrames = 300;  // ~10 seconds of simulation
    
    // Main animation loop
    while (!viewer->wasStopped() && frameCount < maxFrames)
    {
        // Update highway simulation
        highway.stepHighway(egoVelocity, timestamp, frame_per_sec, viewer);
        
        // Render
        viewer->spinOnce(frame_per_sec);
        
        // Update timestamp (in microseconds)
        timestamp += (long long)(1000000.0 / frame_per_sec);
        frameCount++;
    }
    
    // Display final results
    std::cout << std::endl << "======================================" << std::endl;
    std::cout << "  Simulation Complete!" << std::endl;
    std::cout << "======================================" << std::endl;
    
    // Keep viewer open until user closes
    while (!viewer->wasStopped())
    {
        viewer->spinOnce(100);
    }
    
    return 0;
}

