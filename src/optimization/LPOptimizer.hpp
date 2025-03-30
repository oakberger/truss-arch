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
 * Can be extended to MILP later.
 */
class LPOptimizer : public BaseOptimizer {
public:
    void solve(const TrussStructure& structure) override;
    void exportResults(const std::string& filename) override;
};