#include "StrengthConstraint.hpp"
#include "model/TrussStructure.hpp"
#include "solvers/SolverInterface.hpp"
#include "utils/MatrixBuilder.hpp"

void StrengthConstraint::apply(MatrixBuilder& builder,
                               const TrussStructure<2>& structure,
                               const std::vector<int>& free_dofs,
                               SolverInterface& solver) const {
    builder.addStrengthConstraints(structure, solver);
}
