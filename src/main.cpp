#include <iostream>
#include "IO/InputParser.hpp"
#include "IO/OutputExporter.hpp"
#include "optimization/LPOptimizer.hpp"
#include "model/TrussStructure.hpp"
#include "model/OptimizationResults.hpp"

int main() {
    std::cout << "=== Truss Arch Optimizer ===" << std::endl;

    // 1. Load structure from JSON file
    InputParser parser;
    TrussStructure structure = parser.parseFromJSON("input.json");

    // 2. Solve with LPOptimizer
    LPOptimizer optimizer;
    OptimizationResults results = optimizer.solve(structure);

    // 3. Export results
    OutputExporter exporter;
    exporter.exportToSVG(structure, "structure.svg");
    exporter.exportToCSV(results, "results.csv");
    exporter.exportToJSON(results, "results.json");

    std::cout << "✅ Optimization complete. Results exported.\n";
    return 0;
}
