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

    double svg_width = 800.0;
    double aspect_ratio = (max_y - min_y + 1e-6) / (max_x - min_x + 1e-6);
    double svg_height = svg_width * aspect_ratio;

    double padding = 20.0;

    double scale_x = (svg_width - 2 * padding) / (max_x - min_x + 1e-6);
    double scale_y = (svg_height - 2 * padding) / (max_y - min_y + 1e-6);

    auto tx = [&](double x) { return padding + (x - min_x) * scale_x; };
    auto ty = [&](double y) { return svg_height - (padding + (y - min_y) * scale_y); };

    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << svg_width << "\" height=\"" << svg_height << "\">\n";
    file << "<rect width=\"100%\" height=\"100%\" fill=\"white\" />\n";
    file << R"(
        <defs>
          <marker id="arrow" markerWidth="10" markerHeight="10" refX="6" refY="3" orient="auto" markerUnits="strokeWidth">
            <path d="M0,0 L0,6 L9,3 z" fill="blue" />
          </marker>
        </defs>
        )";
    // === 1. Draw inactive bars first (in gray, thin) ===
    for (const auto& result : results.bar_forces) {
        if (!result.is_active) {
            std::size_t bar_id = static_cast<std::size_t>(result.bar_id);
            const auto& bar = truss.bars[bar_id];
    
            std::size_t i = static_cast<std::size_t>(bar.node_start);
            std::size_t j = static_cast<std::size_t>(bar.node_end);
    
            double x1 = tx(model.x_coords[i]);
            double y1 = ty(model.y_coords[i]);
            double x2 = tx(model.x_coords[j]);
            double y2 = ty(model.y_coords[j]);
    
            file << "<line x1=\"" << x1 << "\" y1=\"" << y1
                 << "\" x2=\"" << x2 << "\" y2=\"" << y2
                 << "\" stroke=\"#dddddd\" stroke-width=\"0.5\" stroke-linecap=\"round\" />\n";
        }
    }
    // === 2. Draw active bars on top (in red, scaled width) ===
    for (const auto& result : results.bar_forces) {
        if (result.is_active) {
            std::size_t bar_id = static_cast<std::size_t>(result.bar_id);
            const auto& bar = truss.bars[bar_id];
    
            std::size_t i = static_cast<std::size_t>(bar.node_start);
            std::size_t j = static_cast<std::size_t>(bar.node_end);
    
            double x1 = tx(model.x_coords[i]);
            double y1 = ty(model.y_coords[i]);
            double x2 = tx(model.x_coords[j]);
            double y2 = ty(model.y_coords[j]);
    
            double stroke_width = 0.5 + 2.0 * result.area; // scale factor can be tuned
    
            file << "<line x1=\"" << x1 << "\" y1=\"" << y1
                 << "\" x2=\"" << x2 << "\" y2=\"" << y2
                 << "\" stroke=\"red\" stroke-width=\"" << stroke_width
                 << "\" stroke-linecap=\"round\" />\n";
        }
    }
    
    // === 3. Draw support nodes (as small green triangles or circles) ===
    for (size_t i = 0; i < model.nodeCount(); ++i) {
        bool fix_x = model.support_x[i];
        bool fix_y = model.support_y[i];

        if (fix_x || fix_y) {
            double cx = tx(model.x_coords[i]);
            double cy = ty(model.y_coords[i]);

            std::string symbol = "<circle cx=\"" + std::to_string(cx) +
                                "\" cy=\"" + std::to_string(cy) +
                                "\" r=\"4\" fill=\"green\" stroke=\"black\" stroke-width=\"0.5\" />\n";

            file << symbol;
        }
    }
    // === 4. Draw load arrows (blue lines pointing in force direction) ===
    for (size_t i = 0; i < model.nodeCount(); ++i) {
        double fx = model.load_x[i];
        double fy = model.load_y[i];

        if (std::abs(fx) > 1e-6 || std::abs(fy) > 1e-6) {
            double x = tx(model.x_coords[i]);
            double y = ty(model.y_coords[i]);

            // Simple arrow of fixed length scaled for visual clarity
            double arrow_scale = 1.0;
            double x2 = x - fx * arrow_scale;
            double y2 = y + fy * arrow_scale;

            file << "<line x1=\"" << x2 << "\" y1=\"" << y2
                << "\" x2=\"" << x << "\" y2=\"" << y
                << "\" stroke=\"blue\" stroke-width=\"2\" marker-end=\"url(#arrow)\" />\n";
        }
    }


    file << "</svg>\n";
}



void OutputExporter::exportToCSV(const OptimizationResults& results, const std::string& filepath) {
    std::ofstream file(filepath);
    file << "BarID,Force,Length\n";
    for (const auto& entry : results.bar_forces) {
        file << entry.bar_id << "," << entry.force << "," << entry.length << "," << entry.area << "\n";
    }
}

void OutputExporter::exportToJSON(const OptimizationResults& results, const std::string& filepath) {
    json j;
    for (const auto& entry : results.bar_forces) {
        j["bars"].push_back({
            {"id", entry.bar_id},
            {"force", entry.force},
            {"length", entry.length},
            {"area", entry.area}
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
