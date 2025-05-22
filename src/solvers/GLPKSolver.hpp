#pragma once

#include "../model/TrussStructure.hpp"
#include "SolverInterface.hpp"
#include <glpk.h>
#include <vector>
#include <string>
#include <map>

/**
 * @class GLPKSolver
 * @brief Concrete implementation of SolverInterface using GLPK.
 * 
 * Sets up and solves a linear programming problem using the GLPK backend.
 * Supports constraints and objective functions required for truss optimization.
 */
class GLPKSolver : public SolverInterface {
public:
    /// Constructor initializes the GLPK problem instance.
    GLPKSolver();

    /// Destructor cleans up the GLPK resources.
    ~GLPKSolver();

    /**
     * @brief Initializes the solver with problem-specific parameters.
     * @param num_bars Number of bars (used to set variable layout)
     * @param num_eqs Number of equilibrium equations
     * @param free_dofs List of free degrees of freedom
     * @param structure Reference to the current truss structure
     */
    void initialize(int num_bars,
                    int num_eqs,
                    const std::vector<int>& free_dofs,
                    const TrussStructure<2>& structure) override;

    /**
     * @brief Adds a new variable with bounds.
     * @param name Optional variable name for debugging
     * @param lb Lower bound
     * @param ub Upper bound
     */
    void addVariable(const std::string& name, double lb, double ub) override;

    /**
     * @brief Adds a linear constraint to the LP.
     * @param indices Variable indices used in the constraint
     * @param coeffs Coefficients for each variable
     * @param sense '=', '<', or '>' for constraint type
     * @param rhs Right-hand side value of the constraint
     */
    void addConstraint(const std::vector<int>& indices,
                       const std::vector<double>& coeffs,
                       char sense, double rhs) override;

    /**
     * @brief Sets the LP objective function.
     * @param coeffs Coefficients for each variable
     * @param minimize True to minimize, false to maximize
     */
    void setObjective(const std::vector<double>& coeffs, bool minimize) override;

    /**
     * @brief Solves the current LP problem.
     */
    void solve() override;

    /**
     * @brief Returns the solution vector from the last solve.
     * @return Optimal values for all primal variables
     */
    std::vector<double> getPrimalSolution() const override;

    /**
     * @brief Returns the GLPK status code after solving.
     */
    int getStatus() const override;

private:
    glp_prob* lp;                         ///< Pointer to the GLPK problem instance
    int current_var_index;               ///< Index for next variable
    int current_row_index;               ///< Index for next constraint row

    std::vector<int> ia, ja;             ///< Sparse matrix row/column indices
    std::vector<double> ar;              ///< Sparse matrix values

    int num_bars;                        ///< Number of bars (used to extract forces)
    std::map<std::string, int> variable_map; ///< Optional: name-to-index mapping
    std::vector<double> solution;        ///< Cached solution vector

    const TrussStructure<2>* structure_ref; ///< Pointer to the structure (for length, etc.)
};
