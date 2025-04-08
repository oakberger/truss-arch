#pragma once
#include "BaseOptimizer.hpp"

/**
 * @brief Optimizer based on Linear Programming.
 * 
 * Implements the Ground Structure Method using LP to determine the optimal subset
 * of truss bars. The first working version for MVP.
 * 
 * Optimization Criteria:
 * - Objective: Minimize total material volume (sum of length × area)
 * - Constraints: Equilibrium equations
 * - Bounds: Area ≥ 0
 * 
 * Can (maybe?) be extended to MILP later.
 */
class LPOptimizer : public BaseOptimizer {
public:
    void solve(const TrussStructure& structure) override;
    void exportResults(const std::string& filename) override;
};

/**
 * 
 * Optimization Problem:
 * ---------------------
 * Variables:
 *   - a[i] : Cross-sectional area of bar i (design variable, >= 0)
 *   - q[i] : Axial force in bar i (can be positive or negative)
 * 
 * Given:
 *   - L[i] : Length of bar i
 *   - B    : Equilibrium matrix (A = B)
 *   - f    : External load vector (b = f)
 *   - σ_t  : Max allowable tensile stress
 *   - σ_c  : Max allowable compressive stress
 * 
 * Objective:
 *   Minimize total volume: sum(L[i] * a[i])
 * 
 * Constraints:
 *   1. Equilibrium: B * q = f
 *   2. Stress bounds: -σ_c * a[i] ≤ q[i] ≤ σ_t * a[i]
 *   3. Non-negativity: a[i] ≥ 0
 * 
 * Implementation Plan:
 * ---------------------
 * 1. Build the equilibrium matrix A and load vector b using MatrixBuilder.
 * 2. Choose and initialize an LP solver (GLPK or Gurobi).
 * 3. Add continuous variables:
 *    - a[i] with lower bound 0
 *    - q[i] with free bounds (later constrained via stress bounds)
 * 4. Define the objective function: Minimize sum(L[i] * a[i])
 * 5. Add constraints:
 *    - One row per equilibrium equation (A * q = b)
 *    - Two constraints per bar for q[i] limits based on a[i]
 * 6. Solve the LP problem using the solver.
 * 7. Extract and store the solution into the corresponding bars:
 *    - area → Bar::area
 *    - force → Bar::force
 *    - is_active = (area > ε)
 */
