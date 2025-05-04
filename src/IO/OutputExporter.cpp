#include "OutputExporter.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void OutputExporter::exportToSVG(const TrussStructure& truss,
    const OptimizationResults& results,
    const std::string& filepath)
{
    std::ofstream file(filepath);
    const auto& model = truss.model;

    // --- Bounds for scaling ---
    double min_x = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::lowest();
    double min_y = std::numeric_limits<double>::max();
    double max_y = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < model.nodeCount(); ++i) {
        min_x = std::min(min_x, model.x_coords[i]);
        max_x = std::max(max_x, model.x_coords[i]);
        min_y = std::min(min_y, model.y_coords[i]);
        max_y = std::max(max_y, model.y_coords[i]);
    }

    double width = 800.0;
    double height = 800.0;
    double padding = 20.0;

    double scale_x = (width - 2 * padding) / (max_x - min_x + 1e-6);
    double scale_y = (height - 2 * padding) / (max_y - min_y + 1e-6);

    auto tx = [&](double x) { return padding + (x - min_x) * scale_x; };
    auto ty = [&](double y) { return height - (padding + (y - min_y) * scale_y); };

    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height << "\">\n";
    file << "<rect width=\"100%\" height=\"100%\" fill=\"white\" />\n";

    // === 1. Draw inactive bars first (in gray, thin) ===
    for (const auto& result : results.bar_forces) {
        if (!result.is_active) {
            const auto& bar = truss.bars[result.bar_id];
            double x1 = tx(model.x_coords[bar.node_start]);
            double y1 = ty(model.y_coords[bar.node_start]);
            double x2 = tx(model.x_coords[bar.node_end]);
            double y2 = ty(model.y_coords[bar.node_end]);

            file << "<line x1=\"" << x1 << "\" y1=\"" << y1
            << "\" x2=\"" << x2 << "\" y2=\"" << y2
            << "\" stroke=\"#dddddd\" stroke-width=\"0.5\" stroke-linecap=\"round\" />\n";
        }
    }

    // === 2. Draw active bars on top (in red, scaled width) ===
    for (const auto& result : results.bar_forces) {
        if (result.is_active) {
            const auto& bar = truss.bars[result.bar_id];
            double x1 = tx(model.x_coords[bar.node_start]);
            double y1 = ty(model.y_coords[bar.node_start]);
            double x2 = tx(model.x_coords[bar.node_end]);
            double y2 = ty(model.y_coords[bar.node_end]);

            double abs_force = std::abs(result.force);
            double stroke_width = 1.0 + 3.0 * std::min(abs_force / 100.0, 1.0);  // scale between 1 and 4

            file << "<line x1=\"" << x1 << "\" y1=\"" << y1
            << "\" x2=\"" << x2 << "\" y2=\"" << y2
            << "\" stroke=\"red\" stroke-width=\"" << stroke_width
            << "\" stroke-linecap=\"round\" />\n";
        }
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
