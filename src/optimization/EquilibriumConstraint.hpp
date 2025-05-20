#pragma once
#include "Constraint.hpp"

/**
 * @class EquilibriumConstraint
 * @brief Enforces nodal equilibrium for all free degrees of freedom.
 * 
 * Adds one constraint equation per free DOF to ensure that the
 * internal bar forces balance the external loads.
 */
class EquilibriumConstraint : public Constraint {
public:
    /**
     * @brief Applies equilibrium constraints to the solver.
     * 
     * Constructs and inserts one row per free DOF into the LP
     * system using directional contributions from connected bars.
     *
     * @param builder    Matrix builder utility.
     * @param structure  Truss structure being optimized.
     * @param free_dofs  Indices of unconstrained DOFs.
     * @param solver     LP solver interface to add constraints.
     */
    void apply(MatrixBuilder& builder,
               const TrussStructure<2>& structure,
               const std::vector<int>& free_dofs,
               SolverInterface& solver) const override;
};
