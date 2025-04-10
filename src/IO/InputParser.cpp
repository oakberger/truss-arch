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

    for (const auto& node : j["nodes"]) {
        structure.addNode(node["id"], node["x"], node["y"],
                          node["is_support_x"], node["is_support_y"],
                          node["fx"], node["fy"]);
    }

    for (const auto& bar : j["bars"]) {
        structure.addBar(bar["id"], bar["node_start"], bar["node_end"]);
    }

    return structure;
}
