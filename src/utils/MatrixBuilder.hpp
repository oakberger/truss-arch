#pragma once

#include "solvers/SolverInterface.hpp"
#include "model/TrussStructure.hpp"

/**
 * @class MatrixBuilder
 * @brief Builds constraint matrices for truss optimization problems.
 * 
 * Responsible for assembling equilibrium constraints from the truss structure
 * and inserting them into the solver using the SolverInterface.
 * 
 * Additional builder functions can be added later.
 */
class MatrixBuilder {
public:
    /**
     * @brief Builds the equilibrium constraint matrix.
     * 
     * For each free degree of freedom, computes the contribution of connected bars
     * and inserts one equation into the solver.
     * 
     * @param structure The truss structure to analyze.
     * @param free_dofs List of global free DOF indices.
     * @param solver Reference to the LP solver interface.
     */
    void buildEquilibriumMatrix(const TrussStructure<2>& structure,
                                 const std::vector<int>& free_dofs,
                                 SolverInterface& solver) const;
};
