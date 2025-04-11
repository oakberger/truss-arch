#include <iostream>
#include "geometry/ArchShape.hpp"
#include "model/TrussStructure.hpp"
#include "optimization/LPOptimizer.hpp"
#include "IO/OutputExporter.hpp"
#include "model/OptimizationResults.hpp"

int main() {
    std::cout << "=== Truss Arch Optimizer (TEST MODE) ===" << std::endl;

    // 1. Generate arch-shaped node layout
    ArchShape arch(10.0, 3.0, 0.5); // span, rise, thickness
    std::vector<Node> arch_nodes = arch.generateArchNodes(21, 10, 0.5);

    TrussStructure structure;
    structure.nodes = arch_nodes;

    // 2. Manually apply supports and loads
    structure.nodes[0].is_support_x = true;
    structure.nodes[0].is_support_y = true;
    structure.nodes[20].is_support_y = true;
    structure.nodes[105].fy = -100.0;

    // 3. Generate bars constrained to the arch
    structure.generateGroundStructureArch(2.0, arch);

    // 4. Solve the problem
    LPOptimizer optimizer;
    OptimizationResults results = optimizer.solve(structure);

    // 5. Export the results
    OutputExporter exporter;
    exporter.exportToSVG(structure, "arch_test.svg");
    exporter.exportToCSV(results, "arch_test.csv");
    exporter.exportToJSON(results, "arch_test.json");

    std::cout << "✅ Arch test complete. Results exported.\n";
    return 0;
}
