#include "LPOptimizer.hpp"
#include "Constraint.hpp"
#include "EquilibriumConstraint.hpp"

#include "../utils/MatrixBuilder.hpp"
#include "../solvers/GLPKSolver.hpp"
#include <iostream>
#include <memory>
#include <cmath>

LPOptimizer::LPOptimizer() {
    constraints.push_back(std::make_unique<EquilibriumConstraint>());
}

OptimizationResults LPOptimizer::solve(const TrussStructure<2>& structure) {
    std::cout << "[LPOptimizer] Solving optimization problem using GLPK via SolverInterface...\n";

    const auto& model = structure.model;
    const size_t num_nodes = model.nodeCount();
    const size_t num_bars = structure.bars.size();

    // 1. Determine free DOFs
    std::vector<int> free_dofs;
    for (size_t i = 0; i < num_nodes; ++i) {
        if (!model.support[0][i]) free_dofs.push_back(static_cast<int>(2 * i));
        if (!model.support[1][i]) free_dofs.push_back(static_cast<int>(2 * i + 1));
    }
    std::cout << "[DEBUG] Free DOFs: " << free_dofs.size() << "\n";

    const int num_eq = static_cast<int>(free_dofs.size());

    // 2. Initialize builder and solver interface
    MatrixBuilder builder;
    GLPKSolver solver;
    solver.initialize(static_cast<int>(num_bars), num_eq, free_dofs, structure);

    // 3. Apply modular constraints
    for (const auto& constraint : constraints) {
        constraint->apply(builder, structure, free_dofs, solver);
    }

    // 4. Solve
    solver.solve();
    std::cout << "[LPOptimizer] Solved LP with status: " << solver.getStatus() << "\n";

    // 5. Extract solution
    OptimizationResults results;
    const auto forces = solver.getPrimalSolution();

    for (size_t j = 0; j < num_bars; ++j) {
        double force = forces[j];
        const double sigma = (force >= 0)
            ? structure.material.sigma_tension
            : structure.material.sigma_compression;
        double area = std::abs(force) / sigma;
        results.addEntry(structure.bars[j].id, force, structure.bars[j].length, area);
    }

    results.computeTotalVolume(structure.material.sigma_tension,
                                structure.material.sigma_compression);
    results.markActiveBars();
    return results;
}
