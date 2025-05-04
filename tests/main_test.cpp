#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "IO/InputParser.hpp"
#include "optimization/LPOptimizer.hpp"
#include "model/OptimizationResults.hpp"
#include "IO/OutputExporter.hpp"

using json = nlohmann::json;

int main() {
    std::string input_path = "input/test_case.json";

    try {
        std::ifstream file(input_path);
        json j;
        file >> j;

        // 1. Create parser object
        InputParser parser;

        // 2. Parse input
        TrussStructure structure = parser.parseFromJSON(input_path);

        // 3. Solve optimization
        LPOptimizer optimizer;
        OptimizationResults results = optimizer.solve(structure);

        // 4. Export or print results
        OutputExporter exporter;
        exporter.printToConsole(results);  // optional
        exporter.exportToSVG(structure, results, "output/test_result.svg");

        std::cout << "[Test] Optimization complete.\n";
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}