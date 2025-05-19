#include "EquilibriumConstraint.hpp"
#include "utils/MatrixBuilder.hpp"
#include "model/TrussStructure.hpp"
#include "solvers/SolverInterface.hpp"

void EquilibriumConstraint::apply(MatrixBuilder& builder,
                                  const TrussStructure<2>& structure,
                                  const std::vector<int>& free_dofs,
                                  SolverInterface& solver) const {
    builder.buildEquilibriumMatrix(structure, free_dofs, solver);
}
