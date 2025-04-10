#include "OutputExporter.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void OutputExporter::exportToSVG(const TrussStructure& truss, const std::string& filepath) {
    std::ofstream file(filepath);

    // Very simple SVG header
    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\">\n";

    for (const auto& bar : truss.bars) {
        const auto& n1 = truss.getNode(bar.node_start);
        const auto& n2 = truss.getNode(bar.node_end);
        file << "<line x1=\"" << n1.x << "\" y1=\"" << n1.y
             << "\" x2=\"" << n2.x << "\" y2=\"" << n2.y
             << "\" stroke=\"black\" />\n";
    }

    file << "</svg>\n";
}

void OutputExporter::exportToCSV(const OptimizationResults& results, const std::string& filepath) {
    std::ofstream file(filepath);
    file << "BarID,Force,Length\n";
    for (const auto& entry : results.bar_forces) {
        file << entry.bar_id << "," << entry.force << "," << entry.length << "\n";
    }
}

void OutputExporter::exportToJSON(const OptimizationResults& results, const std::string& filepath) {
    json j;
    for (const auto& entry : results.bar_forces) {
        j["bars"].push_back({
            {"id", entry.bar_id},
            {"force", entry.force},
            {"length", entry.length}
        });
    }

    std::ofstream file(filepath);
    file << j.dump(2);  // pretty print
}
