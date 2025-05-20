#pragma once
#include "../model/TrussStructure.hpp"
#include "../model/OptimizationResults.hpp"

/**
 * @class BaseOptimizer
 * @brief Abstract base class for truss optimization strategies.
 * 
 * Provides a polymorphic interface for implementing optimization algorithms 
 * such as linear programming or other methods.
 * 
 * Responsibilities:
 * - Enforce a uniform interface for all optimizers
 * - Enable flexible extension and integration of multiple strategies
 * 
 * Derived classes must implement:
 * - solve(): Runs the optimization on a given truss structure
 */
class BaseOptimizer {
public:
    virtual ~BaseOptimizer() = default;

    /**
     * @brief Solves the optimization problem for a given truss structure.
     * 
     * Implemented by concrete optimizer classes (e.g., LPOptimizer).
     * 
     * @param structure The truss structure to optimize.
     * @return OptimizationResults containing forces, areas, and volumes.
     */
    virtual OptimizationResults solve(const TrussStructure<2>& structure) = 0;
};
