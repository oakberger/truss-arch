#pragma once

#include "../model/TrussStructure.hpp"
#include "SolverInterface.hpp"
#include <glpk.h>
#include <vector>
#include <string>
#include <map>

class GLPKSolver : public SolverInterface {
public:
    GLPKSolver();
    ~GLPKSolver();

    void initialize(int num_bars,
                    int num_eqs,
                    const std::vector<int>& free_dofs,
                    const TrussStructure<2>& structure);

    void addVariable(const std::string& name, double lb, double ub) override;

    void addConstraint(const std::vector<int>& indices,
                       const std::vector<double>& coeffs,
                       char sense, double rhs) override;

    void setObjective(const std::vector<double>& coeffs, bool minimize) override;

    void solve() override;
    int getStatus() const;

    std::vector<double> getPrimalSolution() const override;

private:
    glp_prob* lp;
    int current_var_index;
    int current_row_index;

    std::vector<int> ia, ja;
    std::vector<double> ar;

    int num_bars;
    std::map<std::string, int> variable_map;
    std::vector<double> solution;

    const TrussStructure<2>* structure_ref;
};
