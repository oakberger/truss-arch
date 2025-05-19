#pragma once

#include "solvers/SolverInterface.hpp"
#include "model/TrussStructure.hpp"

class MatrixBuilder {
public:
    void buildEquilibriumMatrix(const TrussStructure<2>& structure,
                                 const std::vector<int>& free_dofs,
                                 SolverInterface& solver) const;

    void addStrengthConstraints(const TrussStructure<2>& structure,
                                 SolverInterface& solver) const;
};
