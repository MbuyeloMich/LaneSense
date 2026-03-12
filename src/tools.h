#ifndef TOOLS_H_
#define TOOLS_H_
#include <vector>
#include <iostream>
#include "Eigen/Dense"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;

class Tools {
public:
    /**
    * Constructor.
    */
    Tools();
    
    /**
    * Destructor.
    */
    virtual ~Tools();
    
    // Members
    vector<VectorXd> estimations;
    vector<VectorXd> ground_truth;
    
    /**
    * A helper method to calculate RMSE.
    */
    VectorXd CalculateRMSE(const vector<VectorXd> &estimations, const vector<VectorXd> &ground_truth);
    
};

#endif /* TOOLS_H_ */
