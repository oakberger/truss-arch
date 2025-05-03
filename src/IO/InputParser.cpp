#include "InputParser.hpp"
#include <fstream>
#include <nlohmann/json.hpp>  // Example for reading json files as input

using json = nlohmann::json;

// Currently reads Nodes and Bars as input which is helpful for early testing but bypasses GSM logic
// Later input should be just Nodes (Grid size) and boundary conditions such as max. length
TrussStructure InputParser::parseFromJSON(const std::string& filepath) {
    std::ifstream file(filepath);
    json j;
    file >> j;

    TrussStructure structure;

    // Add all nodes first (geometry only)
    for (const auto& node : j["nodes"]) {
        structure.addNode(node["x"], node["y"]);
    }

    // Apply supports and loads after node creation
    for (const auto& node : j["nodes"]) {
        int id = node["id"];
        bool fix_x = node.value("is_support_x", false);
        bool fix_y = node.value("is_support_y", false);
        structure.model.fixNode(id, fix_x, fix_y);

        double fx = node.value("fx", 0.0);
        double fy = node.value("fy", 0.0);
        structure.model.setLoad(id, fx, fy);
    }

    // Add bars
    for (const auto& bar : j["bars"]) {
        structure.addBar(bar["node_start"], bar["node_end"]);
    }

    return structure;
}
