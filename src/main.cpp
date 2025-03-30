#include "model/TrussStructure.hpp"
#include "optimization/LPOptimizer.hpp"
#include "utils/IOUtils.hpp"
#include <iostream>

/**
 * @brief Entry point for the truss arch optimizer.
 * 
 * This example shows how to:
 * - Create a grid of nodes
 * - Generate the ground structure (possible bars)
 * - Run the linear programming optimizer
 * - Export the results (e.g., to SVG)
 */
int main() {
    std::cout << "🔧 Truss Arch Optimizer Starting...\n";

    // 1. Create and initialize the truss structure
    TrussStructure structure;
    structure.generateGrid(3, 5, 1.0); // 3 rows × 5 columns, spacing = 1.0
    structure.generateGroundStructure(2.0); // connect nodes within 2.0 units

    // 2. Apply some loads and supports manually for demonstration
    structure.nodes[0].is_support_x = true;
    structure.nodes[0].is_support_y = true;
    structure.nodes[4].is_support_y = true;
    structure.nodes[7].fy = -100.0; // downward force on node 7

    // 3. Run the optimizer
    LPOptimizer optimizer;
    optimizer.solve(structure);

    // 4. Export the results (dummy export for now)
    optimizer.exportResults("output.svg");

    std::cout << "✅ Optimization complete. Results exported.\n";
    return 0;
}
