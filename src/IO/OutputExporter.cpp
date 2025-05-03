#include "OutputExporter.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void OutputExporter::exportToSVG(const TrussStructure& truss, const std::string& filepath) {
    std::ofstream file(filepath);

    // Very simple SVG header
    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\">\n";

    const auto& model = truss.model;
    for (const auto& bar : truss.bars) {
        double x1 = model.x_coords[bar.node_start];
        double y1 = model.y_coords[bar.node_start];
        double x2 = model.x_coords[bar.node_end];
        double y2 = model.y_coords[bar.node_end];
        file << "<line x1=\"" << x1 << "\" y1=\"" << y1
             << "\" x2=\"" << x2 << "\" y2=\"" << y2
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

void OutputExporter::printToConsole(const OptimizationResults& results) {
    std::cout << "\n[Results] Active Bars:\n";
    for (const auto& entry : results.bar_forces) {
        if (entry.is_active) {
            std::cout << "Bar ID " << entry.bar_id
                      << " | Force = " << entry.force
                      << " | Length = " << entry.length
                      << "\n";
        }
    }
    std::cout << "Total Volume: " << results.total_volume << "\n";
}
