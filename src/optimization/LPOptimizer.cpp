#include "LPOptimizer.hpp"
#include <iostream>

OptimizationResults LPOptimizer::solve(const TrussStructure& structure) {
    std::cout << "[LPOptimizer] Solving optimization problem...\n";

    OptimizationResults results;

    // TODO: Replace this with LP logic
    for (const auto& bar : structure.bars) {
        results.addEntry(bar.id, 1.0, bar.length);  // Dummy force = 1.0
    }

    results.computeTotalVolume();
    results.markActiveBars();

    return results;
}

