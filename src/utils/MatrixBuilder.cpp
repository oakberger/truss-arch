#include "MatrixBuilder.hpp"
#include <Eigen/Sparse>
#include <cmath>

void MatrixBuilder::buildEquilibriumMatrix(const TrussStructure& structure, Eigen::MatrixXd& A, Eigen::VectorXd& b) {
    const auto& model = structure.model;
    const auto& bars = structure.bars;
    const size_t num_nodes = model.nodeCount();
    const size_t num_bars = bars.size();
    
    // 2 DOFs per node (x and y)
    A = Eigen::MatrixXd::Zero(2 * num_nodes, num_bars);
    b = Eigen::VectorXd::Zero(2 * num_nodes);

    for (size_t j = 0; j < num_bars; ++j) {
        int i = bars[j].node_start;
        int k = bars[j].node_end;

        double dx = model.x_coords[k] - model.x_coords[i];
        double dy = model.y_coords[k] - model.y_coords[i];
        double L = std::sqrt(dx * dx + dy * dy);

        if (L == 0.0) continue;  // avoid divide-by-zero

        double cx = dx / L;
        double cy = dy / L;

        // Equilibrium in x
        A(2 * i + 0, j) =  cx;
        A(2 * k + 0, j) = -cx;

        // Equilibrium in y
        A(2 * i + 1, j) =  cy;
        A(2 * k + 1, j) = -cy;
    }

    // RHS vector from external loads
    for (size_t i = 0; i < num_nodes; ++i) {
        b(2 * i + 0) = model.load_x[i];
        b(2 * i + 1) = model.load_y[i];
    }
}