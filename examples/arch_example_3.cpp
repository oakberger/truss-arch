#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "IO/InputParser.hpp"
#include "optimization/LPOptimizer.hpp"
#include "model/OptimizationResults.hpp"
#include "IO/OutputExporter.hpp"

using json = nlohmann::json;

int main() {
    std::string input_path = "examples/input/arch_example_3.json";

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
        exporter.exportToSVG(structure, results, output.value("svg_force", "examples/output/result_force.svg"), false);
        exporter.exportToSVG(structure, results, output.value("svg_area", "examples/output/result_area.svg"), true);
        exporter.exportToCSV(results, output.value("csv", "examples/output/result.csv"));
        exporter.exportToJSON(results, output.value("json", "examples/output/result.json"));

        std::cout << "[Main] Optimization complete.\n";
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}