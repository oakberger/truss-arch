#pragma once
#include <vector>
#include <memory>

template <int dim>
class TrussStructure;
class MatrixBuilder;
class SolverInterface;

/**
 * @class Constraint
 * @brief Abstract interface for defining constraints in truss optimization.
 *
 * Constraints (e.g., equilibrium) implement this interface
 * to contribute equations to the LP system via a matrix builder and solver.
 * 
 * New constraints (e.g., displacement limits, buckling, modularity) can be
 * added by subclassing this and implementing the `apply()` method.
 */
class Constraint {
public:
    /**
     * @brief Apply this constraint to the system.
     *
     * Adds rows to the solver using force balance, strength, or other logic.
     *
     * @param builder    Matrix builder utility.
     * @param structure  Truss structure being optimized.
     * @param free_dofs  Indices of unconstrained DOFs.
     * @param solver     LP solver interface to add constraints.
     */
    virtual void apply(MatrixBuilder& builder,
                       const TrussStructure<2>& structure,
                       const std::vector<int>& free_dofs,
                       SolverInterface& solver) const = 0;

    /// Virtual destructor to support polymorphism.
    virtual ~Constraint() = default;
};
