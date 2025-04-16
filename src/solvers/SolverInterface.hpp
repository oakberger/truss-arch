#pragma once
#include <vector>

/**
 * @brief Interface for Linear Programming (LP) solvers.
 * 
 * This interface defines a standard way to interact with LP solvers such as
 * Gurobi, GLPK, CPLEX, or HiGHS. By using this interface, the rest of the
 * code (like the LPOptimizer or TrussStructure) can work independently
 * of the actual solver being used.
 * 
 * This makes it easy to:
 * - Swap solvers without changing the optimization logic.
 * - Test different solvers for performance or licensing reasons.
 * - Keep the code clean and modular.
 * 
 * Example: You can use GLPK during development (free) and later switch
 * to Gurobi (faster) by simply swapping the implementation.
 */
class SolverInterface {
public:
    /// Virtual destructor to ensure proper cleanup from derived classes.
    virtual ~SolverInterface() {}

    /**
     * @brief Set the objective function coefficients for the LP problem.
     * 
     * The LP problem will try to minimize: c[0]*x0 + c[1]*x1 + ... + c[n]*xn
     * 
     * @param c Vector of objective coefficients.
     */
    virtual void setObjective(const std::vector<double>& c) = 0;

    /**
     * @brief Add a constraint to the LP problem.
     * 
     * Adds a constraint of the form:
     *     a[0]*x0 + a[1]*x1 + ... + a[n]*xn (<=, =, >=) rhs
     * 
     * @param coeffs Coefficients of the constraint (same size as variables).
     * @param rhs Right-hand side of the constraint.
     * @param sense Constraint type: 
     *        'L' = less than or equal (<=),
     *        'E' = equal (=),
     *        'G' = greater than or equal (>=)
     */
    virtual void addConstraint(const std::vector<double>& coeffs, double rhs, char sense) = 0;

    /**
     * @brief Solve the LP problem.
     * 
     * This will call the underlying solver (GLPK, Gurobi, etc.).
     * 
     * @return true if the problem was solved successfully, false otherwise.
     */
    virtual bool solve() = 0;

    /**
     * @brief Get the values of the decision variables after solving.
     * 
     * @return A vector containing the optimal values of the variables.
     */
    virtual std::vector<double> getSolution() const = 0;

    /**
     * @brief Get the final objective value of the solution.
     * 
     * @return The minimized value of the objective function.
     */
    virtual double getObjectiveValue() const = 0;
};
