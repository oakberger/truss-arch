#include "TrussStructure.hpp"
#include <cmath>

void TrussStructure::addNode(double x, double y) {
    model.addNode(x, y);
}

void TrussStructure::addBar(int start_id, int end_id) {
    double dx = model.x_coords[end_id] - model.x_coords[start_id];
    double dy = model.y_coords[end_id] - model.y_coords[start_id];
    double len = std::sqrt(dx*dx + dy*dy);
    bars.emplace_back(bars.size(), start_id, end_id, len);
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
                addBar(i, j);
            }
        }
    }
}

void TrussStructure::generateGroundStructureArch(double max_length, const ArchShape& arch) {
    for (std::size_t i = 0; i < model.x_coords.size(); ++i) {
        for (std::size_t j = i + 1; j < model.x_coords.size(); ++j) {
            double dx = model.x_coords[j] - model.x_coords[i];
            double dy = model.y_coords[j] - model.y_coords[i];
            double dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= max_length &&
                arch.isInside(model.x_coords[i], model.y_coords[i]) &&
                arch.isInside(model.x_coords[j], model.y_coords[j])) {
                addBar(i, j);
            }
        }
    }
}
