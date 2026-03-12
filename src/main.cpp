/* 
 * Headless UKF Highway Demo
 * Runs the Unscented Kalman Filter without visualization
 * Outputs results to console
 */

#include <iostream>
#include <vector>
#include <cmath>
#include "ukf.h"
#include "tools.h"

using namespace std;

// Simple vector struct since we're not using Eigen for this headless version
struct SimpleVector {
    double x, y;
    SimpleVector(double x=0, double y=0) : x(x), y(y) {}
};

struct CarState {
    string name;
    double px, py;      // position
    double v;           // velocity
    double yaw;         // yaw angle
    double yawd;        // yaw rate
    double accel;       // acceleration
    double steering;   // steering angle
    
    CarState(string n, double x, double y, double vel, double yw, double ywd) 
        : name(n), px(x), py(y), v(vel), yaw(yw), yawd(ywd), accel(0), steering(0) {}
};

int main() {
    cout << "======================================" << endl;
    cout << "  Unscented Kalman Filter - Headless" << endl;
    cout << "======================================" << endl;
    cout << endl;
    
    // Initialize tools for RMSE calculation
    Tools tools;
    
    // Create 3 traffic cars
    vector<CarState> traffic;
    traffic.push_back(CarState("car1", -10, 4, 5, 0, 0));
    traffic.push_back(CarState("car2", 25, -4, -6, 0, 0));
    traffic.push_back(CarState("car3", -12, 0, 1, 0, 0));
    
    // Create UKF for each car
    vector<UKF> ukfs;
    for (int i = 0; i < 3; i++) {
        UKF ukf;
        ukfs.push_back(ukf);
    }
    
    // Simulation parameters
    int frame_per_sec = 30;
    int sec_interval = 10;
    int frame_count = 0;
    int time_us = 0;
    double egoVelocity = 25;
    double dt = 1.0 / frame_per_sec;
    
    cout << "Starting simulation..." << endl;
    cout << "Duration: " << sec_interval << " seconds" << endl;
    cout << "Time step: " << dt << " seconds" << endl;
    cout << "Ego car velocity: " << egoVelocity << " m/s" << endl;
    cout << endl;
    
    // Simulation loop
    while (frame_count < (frame_per_sec * sec_interval)) {
        time_us = (int)(1000000.0 * frame_count / frame_per_sec);
        
        // Process each car
        for (int i = 0; i < traffic.size(); i++) {
            // Simulate lidar measurement (ground truth + noise)
            double lidar_noise = 0.1;
            double meas_px = traffic[i].px + ((rand() % 100) / 100.0 - 0.5) * lidar_noise;
            double meas_py = traffic[i].py + ((rand() % 100) / 100.0 - 0.5) * lidar_noise;
            
            // Simulate radar measurement (range, angle, radial velocity)
            double range = sqrt(traffic[i].px * traffic[i].px + traffic[i].py * traffic[i].py);
            double angle = atan2(traffic[i].py, traffic[i].px);
            double radial_vel = traffic[i].v * cos(traffic[i].yaw - angle);
            
            double radar_noise_rho = 0.3;
            double radar_noise_phi = 0.03;
            double radar_noise_rhodot = 0.3;
            double meas_rho = range + ((rand() % 100) / 100.0 - 0.5) * radar_noise_rho;
            double meas_phi = angle + ((rand() % 100) / 100.0 - 0.5) * radar_noise_phi;
            double meas_rhodot = radial_vel + ((rand() % 100) / 100.0 - 0.5) * radar_noise_rhodot;
            
            // Ground truth
            VectorXd gt(4);
            gt << traffic[i].px, traffic[i].py, 
                  traffic[i].v * cos(traffic[i].yaw), 
                  traffic[i].v * sin(traffic[i].yaw);
            tools.ground_truth.push_back(gt);
            
            // Process lidar measurement
            MeasurementPackage lidar_pkg;
            lidar_pkg.sensor_type_ = MeasurementPackage::LASER;
            lidar_pkg.raw_measurements_ = VectorXd(2);
            lidar_pkg.raw_measurements_ << meas_px, meas_py;
            lidar_pkg.timestamp_ = time_us;
            ukfs[i].ProcessMeasurement(lidar_pkg);
            
            // Process radar measurement
            MeasurementPackage radar_pkg;
            radar_pkg.sensor_type_ = MeasurementPackage::RADAR;
            radar_pkg.raw_measurements_ = VectorXd(3);
            radar_pkg.raw_measurements_ << meas_rho, meas_phi, meas_rhodot;
            radar_pkg.timestamp_ = time_us;
            ukfs[i].ProcessMeasurement(radar_pkg);
            
            // Get estimation
            VectorXd estimate(4);
            double v = ukfs[i].x_(2);
            double yaw = ukfs[i].x_(3);
            double v1 = cos(yaw) * v;
            double v2 = sin(yaw) * v;
            estimate << ukfs[i].x_(0), ukfs[i].x_(1), v1, v2;
            tools.estimations.push_back(estimate);
            
            // Move car (simple motion model)
            traffic[i].px += traffic[i].v * cos(traffic[i].yaw) * dt;
            traffic[i].py += traffic[i].v * sin(traffic[i].yaw) * dt;
        }
        
        // Print progress every second
        if (frame_count % frame_per_sec == 0) {
            VectorXd rmse = tools.CalculateRMSE(tools.estimations, tools.ground_truth);
            cout << "Time: " << frame_count / frame_per_sec << "s | RMSE - X: " 
                 << rmse[0] << ", Y: " << rmse[1] 
                 << ", Vx: " << rmse[2] << ", Vy: " << rmse[3] << endl;
        }
        
        frame_count++;
    }
    
    cout << endl;
    cout << "======================================" << endl;
    cout << "  Simulation Complete!" << endl;
    cout << "======================================" << endl;
    
    // Final RMSE
    VectorXd final_rmse = tools.CalculateRMSE(tools.estimations, tools.ground_truth);
    cout << endl << "Final RMSE:" << endl;
    cout << "  X position:  " << final_rmse[0] << " m" << endl;
    cout << "  Y position:  " << final_rmse[1] << " m" << endl;
    cout << "  X velocity:  " << final_rmse[2] << " m/s" << endl;
    cout << "  Y velocity:  " << final_rmse[3] << " m/s" << endl;
    
    // Check if passed threshold
    vector<double> rmseThreshold = {0.30, 0.16, 0.95, 0.70};
    bool pass = true;
    if (final_rmse[0] > rmseThreshold[0]) pass = false;
    if (final_rmse[1] > rmseThreshold[1]) pass = false;
    if (final_rmse[2] > rmseThreshold[2]) pass = false;
    if (final_rmse[3] > rmseThreshold[3]) pass = false;
    
    cout << endl << "Result: " << (pass ? "PASSED" : "FAILED") << " (threshold: 0.30, 0.16, 0.95, 0.70)" << endl;
    
    return 0;
}
