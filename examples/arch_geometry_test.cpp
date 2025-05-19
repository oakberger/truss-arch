#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "IO/InputParser.hpp"
#include "optimization/LPOptimizer.hpp"
#include "model/OptimizationResults.hpp"
#include "IO/OutputExporter.hpp"

using json = nlohmann::json;

int main() {
<<<<<<< HEAD:tests/arch_geometry_test.cpp
    std::string input_path = "input/test_arch_geometry.json";
=======
    std::string input_path = "examples/input/test_arch_geometry.json";
>>>>>>> template:examples/arch_geometry_test.cpp

    try {
        // Load JSON once
        std::ifstream file(input_path);
        if (!file) {
            throw std::runtime_error("Could not open input file: " + input_path);
        }

        json j;
        file >> j;

        // Parse structure using the loaded JSON
        InputParser parser;
        TrussStructure<2> structure = parser.parseFromJSON(j);

        // Solve optimization problem
        LPOptimizer optimizer;
        OptimizationResults results = optimizer.solve(structure);
        results.markActiveBars();

        // Export results
        OutputExporter exporter;
        exporter.printToConsole(results);

        const auto& output = j["output"];
        exporter.exportToSVG(structure, results, output.value("svg_force", "output/result_force.svg"), false);
        exporter.exportToSVG(structure, results, output.value("svg_area", "output/result_area.svg"), true);
        exporter.exportToCSV(results, output.value("csv", "output/result.csv"));
        exporter.exportToJSON(results, output.value("json", "output/result.json"));

        std::cout << "[Main] Optimization complete.\n";
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}