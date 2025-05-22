#pragma once
#include <vector>
#include <string>
#include "../model/TrussStructure.hpp"

/**
 * @class SolverInterface
 * @brief Abstract interface for linear programming solvers.
 * 
 * Defines the minimal API required for setting up and solving LP problems,
 * allowing different backends (e.g. GLPK, Gurobi) to be used interchangeably.
 */
class SolverInterface {
public:
    /**
     * @brief Initializes the LP solver with problem-specific structure.
     * 
     * Typically called before adding constraints or solving.
     * Sets up internal data structures and prepares variable layout.
     * 
     * @param num_bars Number of bars
     * @param num_eqs Number of equilibrium equations
     * @param free_dofs List of degrees of freedom
     * @param structure Reference to the truss structure
     */
    virtual void initialize(int num_bars, int num_eqs,
                            const std::vector<int>& free_dofs,
                            const TrussStructure<2>& structure) = 0;

    /**
     * @brief Adds a new decision variable to the LP problem.
     * @param name Variable name (for debugging)
     * @param lb Lower bound
     * @param ub Upper bound
     */
    virtual void addVariable(const std::string& name, double lb, double ub) = 0;

    /**
     * @brief Adds a linear constraint to the LP.
     * 
     * Format: sum(coeffs[i] * x[indices[i]]) (sense) rhs
     * 
     * @param indices Variable indices
     * @param coeffs Coefficients (same size as indices)
     * @param sense '=', '<', or '>' for equality/inequality
     * @param rhs Right-hand side of the constraint
     */
    virtual void addConstraint(const std::vector<int>& indices,
                               const std::vector<double>& coeffs,
                               char sense,
                               double rhs) = 0;

    /**
     * @brief Retrieves the solver-specific status code after solving.
     * 
     * @return Integer status code returned by the solver backend
     */
    virtual int getStatus() const = 0;

    /**
     * @brief Sets the objective function.
     * @param coeffs Objective coefficients for all variables
     * @param minimize True for minimization (default), false for maximization
     */
    virtual void setObjective(const std::vector<double>& coeffs, bool minimize = true) = 0;

    /**
     * @brief Solves the LP problem.
     */
    virtual void solve() = 0;

    /**
     * @brief Returns the solution vector after solving.
     * @return Optimal values of the decision variables
     */
    virtual std::vector<double> getPrimalSolution() const = 0;

    /// Virtual destructor for polymorphic cleanup
    virtual ~SolverInterface() = default;
};
