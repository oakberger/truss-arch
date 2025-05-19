#pragma once
#include "Constraint.hpp"

class EquilibriumConstraint : public Constraint {
public:
    void apply(MatrixBuilder& builder,
               const TrussStructure<2>& structure,
               const std::vector<int>& free_dofs,
               SolverInterface& solver) const override;
};
