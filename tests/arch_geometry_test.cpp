#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "IO/InputParser.hpp"
#include "optimization/LPOptimizer.hpp"
#include "model/OptimizationResults.hpp"
#include "IO/OutputExporter.hpp"

using json = nlohmann::json;

int main() {
    std::string input_path = "input/test_arch_geometry.json";

    try {
        std::ifstream file(input_path);
        json j;
        file >> j;

        InputParser parser;
        TrussStructure structure = parser.parseFromJSON(input_path);

        LPOptimizer optimizer;
        OptimizationResults results = optimizer.solve(structure);
        results.markActiveBars();

        OutputExporter exporter;
        exporter.printToConsole(results);

        const auto& output = j["output"];
        exporter.exportToSVG(structure, results, output.value("svg", "output/result.svg"));
        exporter.exportToCSV(results, output.value("csv", "output/result.csv"));
        exporter.exportToJSON(results, output.value("json", "output/result.json"));

        std::cout << "[Main] Optimization complete.\n";
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
