#include "InputParser.hpp"
#include <fstream>
#include <nlohmann/json.hpp>  // Example for reading json files as input
#include <iostream>

using json = nlohmann::json;

// Currently reads Nodes and Bars as input which is helpful for early testing but bypasses GSM logic
// Later input should be just Nodes (Grid size) and boundary conditions such as max. length
TrussStructure InputParser::parseFromJSON(const std::string& filepath) {
    std::ifstream file(filepath);
    json j;
    file >> j;

    TrussStructure structure;

    // --- Material Properties ---
    if (j.contains("material")) {
        const auto& mat = j["material"];
        structure.material.sigma_tension = mat.value("sigma_tension", 1.0);
        structure.material.sigma_compression = mat.value("sigma_compression", 1.0);
    }

    // --- Geometry Dispatch ---
    if (j.contains("arch")) {
        const auto& arch = j["arch"];
        double span = arch["span"];
        double rise = arch["rise"];
        double thickness = arch["thickness"];
        int grid_x = arch["grid_x"];
        int grid_y = arch["grid_y"];
        double spacing = arch["spacing"];
        double max_length = arch["max_bar_length"];

        ArchShape archShape(span, rise, thickness);
        archShape.generateArchNodes(structure.model, grid_x, grid_y, spacing);
        structure.generateGroundStructureArch(max_length, archShape);
    }
    else if (j.contains("rectangle")) {
        const auto& rect = j["rectangle"];
        int grid_x = rect["grid_x"];
        int grid_y = rect["grid_y"];
        double spacing = rect["spacing"];
        double max_length = rect["max_bar_length"];

        structure.generateGrid(grid_y, grid_x, spacing);
        structure.generateGroundStructure(max_length);
    }
    else {
        throw std::runtime_error("[InputParser] No recognized geometry block ('arch', 'rectangle') found.");
    }

    std::cout << "[DEBUG] Nodes in model: " << structure.model.nodeCount() << "\n";
    std::cout << "[DEBUG] Bars in model: " << structure.bars.size() << "\n";

    // --- Boundary Conditions ---
    if (j.contains("boundary_conditions")) {
        const auto& bc = j["boundary_conditions"];

        for (const auto& support : bc["supports"]) {
            int id = support["node_id"];
            bool fix_x = support.value("x", false);
            bool fix_y = support.value("y", false);
            structure.model.fixNode(id, fix_x, fix_y);
        }

        for (const auto& load : bc["loads"]) {
            int id = load["node_id"];
            if (id >= structure.model.nodeCount()) {
                throw std::runtime_error("[Error] Load refers to invalid node ID: " + std::to_string(id));
            }
            double fx = load.value("fx", 0.0);
            double fy = load.value("fy", 0.0);
            structure.model.setLoad(id, fx, fy);
        }
    }

    return structure;
}
