#pragma once

#include "SolverInterface.hpp"
#include "../model/TrussStructure.hpp"
#include "gurobi_c++.h"
#include <vector>
#include <string>

/**
 * @class GurobiSolver
 * @brief Concrete implementation of SolverInterface using Gurobi.
 * 
 * Formulates and solves a linear programming problem for truss optimization
 * using the Gurobi solver. Each bar force is represented by a pair of positive
 * and negative variables to handle tension and compression separately.
 */
class GurobiSolver : public SolverInterface {
public:
    /// Constructor initializes the Gurobi environment and model.
    GurobiSolver();

    /// Destructor frees Gurobi resources.
    ~GurobiSolver();

    /**
     * @brief Initializes the solver with structure-specific data and variables.
     * @param num_bars Number of bars in the structure
     * @param num_eqs Number of equilibrium equations (unused)
     * @param free_dofs Free degrees of freedom (unused)
     * @param structure Reference to the truss structure
     */
    void initialize(int num_bars, int num_eqs,
                    const std::vector<int>& free_dofs,
                    const TrussStructure<2>& structure) override;

    /**
     * @brief Adds a linear constraint to the optimization model.
     * @param indices Indices of variables in the constraint
     * @param coeffs Corresponding coefficients
     * @param sense '=', '<', or '>' for constraint type
     * @param rhs Right-hand side value of the constraint
     */
    void addConstraint(const std::vector<int>& indices,
                       const std::vector<double>& coeffs,
                       char sense, double rhs) override;

    /**
     * @brief Solves the current optimization model.
     */
    void solve() override;

    /**
     * @brief Retrieves the optimal primal solution (bar forces).
     * @return Vector of optimal bar forces (positive minus negative)
     */
    std::vector<double> getPrimalSolution() const override;

    /**
     * @brief Returns the Gurobi solver status code.
     * @return Gurobi status code (e.g., GRB_OPTIMAL)
     */
    int getStatus() const override;

    /**
     * @brief Not used in this solver. Throws error if called.
     */
    void addVariable(const std::string& name, double lb, double ub) override;

    /**
     * @brief Not used in this solver. Throws error if called.
     */
    void setObjective(const std::vector<double>& coeffs, bool minimize = true) override;

private:
    GRBEnv env;                          ///< Gurobi environment
    GRBModel model;                      ///< Gurobi model

    std::vector<GRBVar> force_pos_vars;  ///< Variables for tensile forces
    std::vector<GRBVar> force_neg_vars;  ///< Variables for compressive forces
    int num_bars = 0;                    ///< Number of bars
    const TrussStructure<2>* structure_ref = nullptr; ///< Reference to the structure
    int gurobi_status = -1;              ///< Solver status after optimization
};
