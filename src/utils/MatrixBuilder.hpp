#pragma once
#include "../model/TrussStructure.hpp"
#include <Eigen/Dense>

/**
 * @brief Builds the global equilibrium matrix and load vector.
 * 
 * Forms the core of the Ground Structure Method. Responsible for:
 * - Calculating bar direction cosines
 * - Assembling matrix A (equilibrium)
 * - Assembling vector b (external loads)
 * 
 * System: A * f = b
 */
class MatrixBuilder {
public:
    static void buildEquilibriumMatrix(const TrussStructure<2>& structure, Eigen::MatrixXd& A, Eigen::VectorXd& b);
};