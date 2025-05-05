#include "ParallelArchShape.hpp"
#include <cmath>
#include <iostream>

ParallelArchShape::ParallelArchShape(double span, double rise, double thickness)
    : span_(span), rise_(rise), thickness_(thickness) {}

double ParallelArchShape::outerY(double x) const {
    double a = -4.0 * rise_ / (span_ * span_);
    return a * std::pow(x - span_ / 2.0, 2.0) + rise_;
}

double ParallelArchShape::innerY(double x) const {
    return outerY(x) - thickness_;
}

bool ParallelArchShape::isInside(double x, double y) const {
    double top = outerY(x);
    double bottom = innerY(x);
    return y >= bottom && y <= top;
}


void ParallelArchShape::generateArchNodes(Model& model, int num_arch_nodes, int num_vertical_layers) const {
    for (int i = 0; i < num_arch_nodes; ++i) {
        double x = span_ * i / (num_arch_nodes - 1);  // evenly along the span

        double y_outer = outerY(x);
        double y_inner = innerY(x);

        for (int j = 0; j < num_vertical_layers; ++j) {
            double t = (num_vertical_layers == 1) ? 0.0 : static_cast<double>(j) / (num_vertical_layers - 1);
            double y = y_inner + t * (y_outer - y_inner);  // linear interpolation

            size_t node_id = model.addNode(x, y);
            std::cout << "[DEBUG] Added node " << node_id << " at (" << x << ", " << y << ")\n";
        }
    }
}
