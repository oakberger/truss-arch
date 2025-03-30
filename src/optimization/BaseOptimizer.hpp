#pragma once
#include "../model/TrussStructure.hpp"

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
class BaseOptimizer {
public:
    virtual void solve(const TrussStructure& structure) = 0;
    virtual void exportResults(const std::string& filename) = 0;
    virtual ~BaseOptimizer() {}
};