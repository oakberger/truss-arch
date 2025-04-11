#include "TrussStructure.hpp"
#include <cmath>

void TrussStructure::addNode(double x, double y) {
    nodes.emplace_back(nodes.size(), x, y);
}

void TrussStructure::addBar(int start_id, int end_id) {
    double dx = nodes[end_id].x - nodes[start_id].x;
    double dy = nodes[end_id].y - nodes[start_id].y;
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
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        for (std::size_t j = i + 1; j < nodes.size(); ++j) {
            double dx = nodes[j].x - nodes[i].x;
            double dy = nodes[j].y - nodes[i].y;
            double dist = std::sqrt(dx*dx + dy*dy);
            if (dist <= max_length) {
                addBar(i, j);
            }
        }
    }
}

void TrussStructure::generateGroundStructureArch(double max_length, const ArchShape& arch) {
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        for (std::size_t j = i + 1; j < nodes.size(); ++j) {
            double dx = nodes[j].x - nodes[i].x;
            double dy = nodes[j].y - nodes[i].y;
            double dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= max_length &&
                arch.isInside(nodes[i].x, nodes[i].y) &&
                arch.isInside(nodes[j].x, nodes[j].y)) {
                addBar(i, j);
            }
        }
    }
}
