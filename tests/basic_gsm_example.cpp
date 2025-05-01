

#include "../src/model/TrussStructure.hpp"
#include "../src/utils/MatrixBuilder.hpp"
#include "../src/optimization/LPOptimizer.hpp"
#include "../src/io/OutputExporter.hpp"

#include <iostream>
#include <Eigen/Dense>

int main() {
    TrussStructure structure;

    // Define a simple 2x1 node grid
    structure.addNode(0.0, 0.0); // Node 0
    structure.addNode(1.0, 0.0); // Node 1
    structure.addNode(0.5, 1.0); // Node 2

    // Supports
    structure.model.support_x[0] = true;
    structure.model.support_y[0] = true;
    structure.model.support_y[1] = true;

    // Loads
    structure.model.load_y[2] = -1.0;

    // Bars (triangle)
    structure.addBar(0, 1);
    structure.addBar(0, 2);
    structure.addBar(1, 2);

    // Build A and b
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    MatrixBuilder::buildEquilibriumMatrix(structure, A, b);

    std::cout << "Equilibrium matrix A:\n" << A << "\n\n";
    std::cout << "Load vector b:\n" << b.transpose() << "\n";

    return 0;
}