// Handle logic for creating traffic on highway and animating it

#include "render/render.h"
#include "ukf.h"
#include "tools.h"
#include "sensors/lidar.h"
#include <cmath>
#include <cstdlib>

#include "sensors/lidar.h"
#include "tools.h"
#include "ukf.h"

class Highway
{
public:

	std::vector<Car> traffic;
	Car egoCar;
	Tools tools;
	bool pass = true;
	std::vector<double> rmseThreshold = {0.30,0.16,0.95,0.70};
	std::vector<double> rmseFailLog = {0.0,0.0,0.0,0.0};
	Lidar* lidar;
	
	// UKF objects for each car
	std::vector<UKF> ukfs;
	
	// Parameters 
	// --------------------------------
	// Set which cars to track with UKF
	std::vector<bool> trackCars = {true,true,true};
	// Visualize sensor measurements
	bool visualize_lidar = true;
	bool visualize_radar = true;
	bool visualize_pcd = false;
	// Predict path in the future using UKF
	double projectedTime = 0;
	int projectedSteps = 0;
	// --------------------------------

	Highway(pcl::visualization::PCLVisualizer::Ptr& viewer)
	{

		tools = Tools();
	
		// Create ego car - green, center lane
		egoCar = Car(Vect3(0, 0, 0), Vect3(4, 2, 2), Color(0, 1, 0), "egoCar");
		egoCar.velocity = 25;
		egoCar.angle = 0;
		
		// Initialize UKF for ego
		UKF egoUKF;
		ukfs.push_back(egoUKF);
		
		// Create traffic cars with different behaviors
		
		// Car 1 - ahead in left lane, slower (will be overtaken)
		Car car1(Vect3(-10, 4, 0), Vect3(4, 2, 2), Color(0, 0, 1), "car1");
		car1.velocity = 20;  // slower than ego
		car1.angle = 0;
		
		// Car 2 - oncoming in right lane
		Car car2(Vect3(25, -4, 0), Vect3(4, 2, 2), Color(0, 0, 1), "car2");
		car2.velocity = 30;  // fast oncoming
car2.angle = 3.14159265;  // opposite direction
		
		// Car 3 - behind in center lane, faster (will overtake)
		Car car3(Vect3(-12, 0, 0), Vect3(4, 2, 2), Color(0, 0, 1), "car3");
		car3.velocity = 28;  // faster than ego - will overtake
		car3.angle = 0;
		
		traffic.push_back(car1);
		traffic.push_back(car2);
		traffic.push_back(car3);
		
		// Initialize UKF for each traffic car
		for(size_t i = 0; i < traffic.size(); i++)
		{
			UKF ukf;
			ukfs.push_back(ukf);
			traffic[i].ukf = &ukfs[i];
		}
		
		// Also attach UKF to egoCar
		egoCar.ukf = &ukfs[0];

		lidar = new Lidar(traffic, 0);
	}
	
	void stepHighway(double egoVelocity, long long timestamp, int frame_per_sec, pcl::visualization::PCLVisualizer::Ptr& viewer)
	{

		// Visualize the highway
		viewer->addLine(pcl::PointXYZ(-100, -4, 0), pcl::PointXYZ(100, -4, 0), 1, 1, 1, "line1");
		viewer->addLine(pcl::PointXYZ(-100, 4, 0), pcl::PointXYZ(100, 4, 0), 1, 1, 1, "line2");
		
		// Render ego car
		egoCar.render(viewer);
		
		double dt = (double)1 / frame_per_sec;
		
		// Process each traffic car
		for (size_t i = 0; i < traffic.size(); i++)
		{
			// Move car based on velocity and angle
			traffic[i].position.x += traffic[i].velocity * cos(traffic[i].angle) * dt;
			traffic[i].position.y += traffic[i].velocity * sin(traffic[i].angle) * dt;
			
			// Wrap around for continuous simulation
			if (traffic[i].position.x > 60) traffic[i].position.x = -60;
			if (traffic[i].position.x < -60) traffic[i].position.x = 60;
			
			// Render traffic car
			traffic[i].render(viewer);
			
			// Get UKF for this car
			UKF* ukf = (UKF*)traffic[i].ukf;
			
			// Ground truth - actual car position and velocity
			Eigen::VectorXd gt(4);
			gt << traffic[i].position.x, traffic[i].position.y, 
				  traffic[i].velocity*cos(traffic[i].angle), 
				  traffic[i].velocity*sin(traffic[i].angle);
			
			// Lidar detection (red sphere)
			if(visualize_lidar)
			{
				// Add noise to lidar measurement
				double noise = 0.15;
				double lidarX = traffic[i].position.x + noise * (((double) rand() / (RAND_MAX)) - 0.5);
				double lidarY = traffic[i].position.y + noise * (((double) rand() / (RAND_MAX)) - 0.5);
				
				viewer->addSphere(pcl::PointXYZ(lidarX, lidarY, 0.5), 0.5, 1, 0, 0, 
					"lidar_" + std::to_string(timestamp) + "_" + std::to_string(i));
			}
			
			// Radar detection (purple line)
			if(visualize_radar)
			{
				viewer->addLine(pcl::PointXYZ(egoCar.position.x, egoCar.position.y, 0.5),
							   pcl::PointXYZ(traffic[i].position.x, traffic[i].position.y, 0.5),
							   0.5, 0, 0.5, "radar_" + std::to_string(timestamp) + "_" + std::to_string(i));
			}
			
			// UKF estimation
			Eigen::VectorXd estimate(4);
			
			if(trackCars[i])
			{
				// Use UKF state as estimate
				double v = ukf->x_(2);
				double yaw = ukf->x_(3);
				double vx = v * cos(yaw);
				double vy = v * sin(yaw);
				
				estimate << ukf->x_(0), ukf->x_(1), vx, vy;
				
				tools.ground_truth.push_back(gt);
				tools.estimations.push_back(estimate);
			}
			
			// Visualize UKF predicted path if enabled
			if(projectedSteps > 0)
			{
				for(int j = 1; j <= projectedSteps; j++)
				{
					double t = projectedTime * j / projectedSteps;
					double predX = ukf->x_(0) + ukf->x_(2) * cos(ukf->x_(3)) * t;
					double predY = ukf->x_(1) + ukf->x_(2) * sin(ukf->x_(3)) * t;
					
					viewer->addSphere(pcl::PointXYZ(predX, predY, 0.5), 0.2, 0, 1, 0, 
						"ukf_pred_" + std::to_string(i) + "_" + std::to_string(j));
				}
			}
		}
		
		// Calculate and display RMSE
		viewer->addText("Accuracy - RMSE:", 30, 300, 20, 1, 1, 1, "rmse");
		
		if(tools.estimations.size() > 0 && tools.ground_truth.size() > 0)
		{
			Eigen::VectorXd rmse = tools.CalculateRMSE(tools.estimations, tools.ground_truth);
			viewer->addText(" X: " + std::to_string(rmse[0]), 30, 275, 20, 1, 1, 1, "rmse_x");
			viewer->addText(" Y: " + std::to_string(rmse[1]), 30, 250, 20, 1, 1, 1, "rmse_y");
			viewer->addText("Vx: " + std::to_string(rmse[2]), 30, 225, 20, 1, 1, 1, "rmse_vx");
			viewer->addText("Vy: " + std::to_string(rmse[3]), 30, 200, 20, 1, 1, 1, "rmse_vy");
			
			// Check if RMSE exceeds threshold
			if(rmse[0] > rmseThreshold[0] || rmse[1] > rmseThreshold[1] || 
			   rmse[2] > rmseThreshold[2] || rmse[3] > rmseThreshold[3])
			{
				pass = false;
			}
		}
	}
};
