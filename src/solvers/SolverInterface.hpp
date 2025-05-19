#pragma once
#include <vector>
#include <string>

class SolverInterface {
public:
    virtual void addVariable(const std::string& name, double lb, double ub) = 0;

    virtual void addConstraint(const std::vector<int>& indices,
                                const std::vector<double>& coeffs,
                                char sense,  // '=', '<', or '>'
                                double rhs) = 0;

    virtual void setObjective(const std::vector<double>& coeffs, bool minimize = true) = 0;
    virtual void solve() = 0;

    virtual std::vector<double> getPrimalSolution() const = 0;

    virtual ~SolverInterface() = default;
};
