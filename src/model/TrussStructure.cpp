#include "TrussStructure.hpp"
#include <cmath>

void TrussStructure::addNode(double x, double y, bool support_x, bool support_y) {
    nodes.emplace_back(nodes.size(), x, y, support_x, support_y);
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
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = i + 1; j < nodes.size(); ++j) {
            double dx = nodes[j].x - nodes[i].x;
            double dy = nodes[j].y - nodes[i].y;
            double dist = std::sqrt(dx*dx + dy*dy);
            if (dist <= max_length) {
                addBar(i, j);
            }
        }
    }
}