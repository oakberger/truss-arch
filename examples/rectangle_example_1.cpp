#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "IO/InputParser.hpp"
#include "optimization/LPOptimizer.hpp"
#include "IO/OutputExporter.hpp"
#include "solvers/GLPKSolver.hpp"

using json = nlohmann::json;

int main() {
    std::string input_path = "examples/input/rectangle_1.json";

    try {
        std::ifstream file(input_path);
        if (!file) throw std::runtime_error("Could not open input file: " + input_path);

        json j;
        file >> j;

        InputParser parser;
        TrussStructure<2> structure = parser.parseFromJSON(j);

        LPOptimizer optimizer;
        GLPKSolver solver;
        OptimizationResults results = optimizer.solve(structure, solver);
        results.markActiveBars();

        OutputExporter exporter;
        exporter.printToConsole(results);

        const auto& output = j["output"];
        exporter.exportToSVG(structure, results, output.value("svg", "output/result_force.svg"), false);
        exporter.exportToSVG(structure, results, output.value("svg_area", "output/result_area.svg"), true);
        exporter.exportToCSV(results, output.value("csv", "output/result.csv"));
        exporter.exportToJSON(results, output.value("json", "output/result.json"));

        std::cout << "[Main] Optimization complete.\n";
    } catch (const std::exception& e) { // catch all standard exceptions
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
