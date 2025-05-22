#pragma once

#include "SolverInterface.hpp"
#include "../model/TrussStructure.hpp"
#include "gurobi_c++.h"
#include <vector>

class GurobiSolver : public SolverInterface {
public:
    GurobiSolver();
    ~GurobiSolver();

    void initialize(int num_bars, int num_eqs,
                    const std::vector<int>& free_dofs,
                    const TrussStructure<2>& structure) override;

    void addConstraint(const std::vector<int>& indices,
                       const std::vector<double>& coeffs,
                       char sense, double rhs) override;

    void solve() override;
    std::vector<double> getPrimalSolution() const override;
    int getStatus() const override;

    void addVariable(const std::string& name, double lb, double ub) override;
    void setObjective(const std::vector<double>& coeffs, bool minimize = true) override;


private:
    GRBEnv env;
    GRBModel model;

    std::vector<GRBVar> force_pos_vars;
    std::vector<GRBVar> force_neg_vars;
    int num_bars = 0;
    const TrussStructure<2>* structure_ref = nullptr;
    int gurobi_status = -1;
};
