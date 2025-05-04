#include "ArchShape.hpp"
#include <cmath>
#include <iostream>

ArchShape::ArchShape(double span, double rise, double thickness)
    : span_(span), rise_(rise), thickness_(thickness) {}

double ArchShape::outerY(double x) const {
    double a = -4.0 * rise_ / (span_ * span_);
    return a * std::pow(x - span_ / 2.0, 2.0) + rise_;
}

double ArchShape::innerY(double x) const {
    return outerY(x) - thickness_;
}

bool ArchShape::isInside(double x, double y) const {
    double top = outerY(x);
    double bottom = innerY(x);
    return y >= bottom && y <= top;
}


void ArchShape::generateArchNodes(Model& model, int nx, int ny, double spacing) const {
    for (int i = 0; i < nx; ++i) {
        double x = i * spacing;
        for (int j = 0; j < ny; ++j) {
            double y = j * spacing;
            if (isInside(x, y)) {
                size_t node_id = model.addNode(x, y);
                std::cout << "[DEBUG] Added node " << node_id << " at (" << x << ", " << y << ")\n";
            }
        }
    }
}
