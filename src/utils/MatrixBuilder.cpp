#include "MatrixBuilder.hpp"
#include <Eigen/Sparse>
#include <cmath>

void MatrixBuilder::buildEquilibriumMatrix(const TrussStructure<2>& structure, Eigen::MatrixXd& A, Eigen::VectorXd& b) {
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

        double dx = model.coords[0][k] - model.coords[0][i];
        double dy = model.coords[1][k] - model.coords[1][i];
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
        b(static_cast<int>(2 * i + 0)) = model.load[0][i];
        b(static_cast<int>(2 * i + 1)) = model.load[1][i];
    }
}
