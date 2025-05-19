#pragma once
#include <vector>
#include <memory>

template <int dim>
class TrussStructure;
class MatrixBuilder;
class SolverInterface;

class Constraint {
public:
    virtual void apply(MatrixBuilder& builder,
                       const TrussStructure<2>& structure,
                        const std::vector<int>& free_dofs,
                       SolverInterface& solver) const =0;

    virtual ~Constraint() = default;
};
