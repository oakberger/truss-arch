#include "../src/IO/InputParser.hpp"
#include "../src/optimization/LPOptimizer.hpp"
#include "../src/IO/OutputExporter.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    std::string input_path = "input/truss_arch_config.json";

    try {
        // Load full JSON so we can extract output paths
        std::ifstream file(input_path);
        json j;
        file >> j;

        // Step 1: Parse structure from JSON
        InputParser parser;
        TrussStructure structure = parser.parseFromJSON(input_path);

        // Step 2: Solve optimization
        LPOptimizer optimizer;
        OptimizationResults results = optimizer.solve(structure);

        // Step 3: Get output paths from JSON
        std::string svg_path = j["output"].value("svg", "output/result.svg");
        std::string csv_path = j["output"].value("csv", "output/result.csv");
        std::string json_path = j["output"].value("json", "output/result.json");

        // Step 4: Export results
        OutputExporter exporter;
        exporter.exportToSVG(structure, results, svg_path);
        exporter.exportToCSV(results, csv_path);
        exporter.exportToJSON(results, json_path);
        exporter.printToConsole(results);

        std::cout << "[Main] Optimization complete.\n";
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
