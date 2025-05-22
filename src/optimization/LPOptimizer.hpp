#pragma once
#include "Constraint.hpp"
#include "BaseOptimizer.hpp"

/**
 * @class LPOptimizer
 * @brief Truss optimizer using Linear Programming (LP).
 *
 * Solves the layout problem using the Ground Structure Method by minimizing
 * material volume under equilibrium and strength constraints.
 * 
 * - Objective: Minimize total volume (∑ |Fᵢ|·Lᵢ / σ)
 * - Constraints: Modular (e.g. equilibrium, more can be added)
 * - Extensible: Add custom constraints via Constraint subclasses.
 */
class LPOptimizer : public BaseOptimizer {
private:
    /// List of modular constraints to apply (e.g. equilibrium, strength)
    std::vector<std::unique_ptr<Constraint>> constraints;

public:
    /// Constructs the optimizer and adds default constraints
    LPOptimizer();

    /**
     * @brief Solves the truss optimization problem using GLPK.
     * 
     * Applies all registered constraints, solves the LP problem, 
     * and returns force and area results per bar.
     * 
     * @param structure The truss structure to optimize.
     * @param solver The linear solver to use (e.g., GLPKSolver, GurobiSolver).
     * @return OptimizationResults containing bar forces and volume.
     */
    OptimizationResults solve(const TrussStructure<2>& structure, SolverInterface& solver) override;
};
