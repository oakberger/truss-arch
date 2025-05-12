#include "TrussStructure.hpp"
#include <cmath>
#include <cassert>

size_t TrussStructure::addNode(double x, double y) {
   return model.addNode(x, y);
}

void TrussStructure::addBar(std::size_t start_id, std::size_t end_id) {
    assert(start_id >= 0 && end_id >= 0);

    std::size_t i = static_cast<std::size_t>(start_id);
    std::size_t j = static_cast<std::size_t>(end_id);

    double dx = model.x_coords[j] - model.x_coords[i];
    double dy = model.y_coords[j] - model.y_coords[i];
    double len = std::sqrt(dx * dx + dy * dy);

    bars.emplace_back(bars.size(), static_cast<int>(start_id), static_cast<int>(end_id), len);  // keep original IDs
}

void TrussStructure::generateGrid(int rows, int cols, double spacing) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            addNode(j * spacing, i * spacing);
        }
    }
}

void TrussStructure::generateGroundStructure(double max_length) {
    for (std::size_t i = 0; i < model.x_coords.size(); ++i) {
        for (std::size_t j = i + 1; j < model.x_coords.size(); ++j) {
            double dx = model.x_coords[j] - model.x_coords[i];
            double dy = model.y_coords[j] - model.y_coords[i];
            double dist = std::sqrt(dx*dx + dy*dy);
            if (dist <= max_length) {
                addBar(static_cast<int>(i), static_cast<int>(j));
            }
        }
    }
}


void TrussStructure::generateGroundStructureArch(double max_length, const BaseArchShape& arch) {
    for (std::size_t i = 0; i < model.x_coords.size(); ++i) {
        for (std::size_t j = i + 1; j < model.x_coords.size(); ++j) {
            double dx = model.x_coords[j] - model.x_coords[i];
            double dy = model.y_coords[j] - model.y_coords[i];
            double dist = std::sqrt(dx * dx + dy * dy);

            double mx = 0.5 * (model.x_coords[i] + model.x_coords[j]);
            double my = 0.5 * (model.y_coords[i] + model.y_coords[j]);

            if (dist <= max_length &&
                arch.isInside(model.x_coords[i], model.y_coords[i]) &&
                arch.isInside(model.x_coords[j], model.y_coords[j]) &&
                arch.isInside(mx, my)) {
                addBar(static_cast<int>(i), static_cast<int>(j));
            }
        }
    }
}
