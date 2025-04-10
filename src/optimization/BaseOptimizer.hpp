#pragma once
#include "../model/TrussStructure.hpp"
#include "../model/OptimizationResults.hpp"

/**
 * @brief Abstract base class for all optimization strategies.
 * 
 * Provides a polymorphic interface for truss layout optimizers.
 * Makes the system extendable and enforces clean separation of logic.
 * 
 * Derived classes must implement:
 * - solve(): solve the optimization problem
 * - exportResults(): output the final design (e.g., to file)
 */
/**
 * @brief Base class for optimization solvers.
 */
class BaseOptimizer {
    public:
        virtual ~BaseOptimizer() = default;
    
        /**
         * @brief Solves the optimization problem.
         * @param structure The truss structure to optimize.
         * @return The results of the optimization.
         */
        virtual OptimizationResults solve(const TrussStructure& structure) = 0;
    
    };