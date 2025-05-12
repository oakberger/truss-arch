#include "MatrixBuilder.hpp"
#include <Eigen/Sparse>
#include <cmath>

void MatrixBuilder::buildEquilibriumMatrix(const TrussStructure& structure, Eigen::MatrixXd& A, Eigen::VectorXd& b) {
    const auto& model = structure.model;
    const auto& bars = structure.bars;
    const std::size_t num_nodes = model.nodeCount();
    const std::size_t num_bars = bars.size();
    
    // 2 DOFs per node (x and y)
    A = Eigen::MatrixXd::Zero(static_cast<int>(2 * num_nodes), static_cast<int>(num_bars));
    b = Eigen::VectorXd::Zero(static_cast<int>(2 * num_nodes));

    for (std::size_t j = 0; j < num_bars; ++j) {
        std::size_t i = static_cast<std::size_t>(bars[j].node_start);
        std::size_t k = static_cast<std::size_t>(bars[j].node_end);

        double dx = model.x_coords[k] - model.x_coords[i];
        double dy = model.y_coords[k] - model.y_coords[i];
        double L = std::sqrt(dx * dx + dy * dy);

        if (L == 0.0) continue;  // avoid divide-by-zero

        double cx = dx / L;
        double cy = dy / L;

        // Equilibrium in x
        A(static_cast<int>(2 * i + 0), static_cast<int>(j)) =  cx;
        A(static_cast<int>(2 * k + 0), static_cast<int>(j)) = -cx;

        // Equilibrium in y
        A(static_cast<int>(2 * i + 1), static_cast<int>(j)) =  cy;
        A(static_cast<int>(2 * k + 1), static_cast<int>(j)) = -cy;
    }

    // RHS vector from external loads
    for (std::size_t i = 0; i < num_nodes; ++i) {
        b(static_cast<int>(2 * i + 0)) = model.load_x[i];
        b(static_cast<int>(2 * i + 1)) = model.load_y[i];
    }
}
