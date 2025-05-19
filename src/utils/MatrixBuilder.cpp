#include "MatrixBuilder.hpp"
#include <cmath>
#include <iostream>

void MatrixBuilder::buildEquilibriumMatrix(const TrussStructure<2>& structure,
                                           const std::vector<int>& free_dofs,
                                           SolverInterface& solver) const
{
    const auto& model = structure.model;
    const auto& bars = structure.bars;
    const int num_bars = static_cast<int>(bars.size());

    // For each free DOF, build an equilibrium equation
    for (size_t row = 0; row < free_dofs.size(); ++row) {
        int global_dof = free_dofs[row];
        int node = global_dof / 2;
        bool is_x = (global_dof % 2 == 0);

        std::vector<int> indices;
        std::vector<double> coeffs;

        for (int j = 0; j < num_bars; ++j) {
            const auto& bar = bars[j];
            int i = bar.node_start;
            int k = bar.node_end;

            double dx = model.coords[0][k] - model.coords[0][i];
            double dy = model.coords[1][k] - model.coords[1][i];
            double L = bar.length;
            double cx = dx / L;
            double cy = dy / L;

            double c = is_x ? cx : cy;

            // Check if bar connects to this node
            if (node == i) {
                indices.push_back(j);             // F⁺
                coeffs.push_back( c);

                indices.push_back(j + num_bars);  // F⁻
                coeffs.push_back(-c);
            }
            if (node == k) {
                indices.push_back(j);             // F⁺
                coeffs.push_back(-c);

                indices.push_back(j + num_bars);  // F⁻
                coeffs.push_back( c);
            }
        }

        // RHS is external load
        double rhs = is_x ? model.load[0][node] : model.load[1][node];

        solver.addConstraint(indices, coeffs, '=', rhs);
        std::cout << "[Row " << row << "] ";
        for (size_t k = 0; k < indices.size(); ++k)
            std::cout << coeffs[k] << "*x" << indices[k] << " ";
        std::cout << "= " << rhs << "\n";

    }
}

void MatrixBuilder::addStrengthConstraints(const TrussStructure<2>& structure,
                                           SolverInterface& solver) const
{
    const auto& bars = structure.bars;
    const double σ_t = structure.material.sigma_tension;
    const double σ_c = structure.material.sigma_compression;

    for (int j = 0; j < static_cast<int>(bars.size()); ++j) {
        const auto& bar = bars[j];

        std::vector<int> indices = {j, j + static_cast<int>(bars.size())}; // F⁺ and F⁻
        std::vector<double> coeffs = {1.0 / σ_t, 1.0 / σ_c};               // stress bounds

        solver.addConstraint(indices, coeffs, '=', bar.area);  // placeholder: area is not known here
        // In reality, this constraint may be handled implicitly via objective or MILP logic
    }
}
