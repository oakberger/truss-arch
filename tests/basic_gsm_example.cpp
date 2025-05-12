

#include "../src/model/TrussStructure.hpp"
#include "../src/utils/MatrixBuilder.hpp"
#include "../src/optimization/LPOptimizer.hpp"
#include "../src/IO/OutputExporter.hpp"

#include <iostream>
#include <Eigen/Dense>

int main() {
    TrussStructure<2> structure;

    // Define a simple 2x1 node grid
    size_t n0 = structure.addNode({0.0, 0.0}); // Node 0
    size_t n1 = structure.addNode({0.0, 6.0}); // Node 1
    size_t n2 = structure.addNode({-1.0, 3.0}); // Node 2
    size_t n3 = structure.addNode({1.0, 3.0}); // Node 3

    // Supports
    structure.model.fixNode(n0, {true, true}); // Fixed in x and y
    structure.model.fixNode(n1, {true, false}); // Fixed only in x
    // Loads
    structure.model.setLoad(n1, {0.0, -100.0}); // Vertical load

    // Bars (triangle)
    structure.addBar(n0, n1);
    structure.addBar(n0, n2);
    structure.addBar(n0, n3);
    structure.addBar(n1, n2);
    structure.addBar(n1, n3);
    structure.addBar(n2, n3);

    // Build A and b
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    MatrixBuilder::buildEquilibriumMatrix(structure, A, b);

    std::cout << "Equilibrium matrix A:\n" << A << "\n\n";
    std::cout << "Load vector b:\n" << b.transpose() << "\n";

    // Solve using LP optimizer
    LPOptimizer optimizer;
    OptimizationResults results = optimizer.solve(structure);

    // Print results to console
    OutputExporter::printToConsole(results);

    return 0;
}