#include "EllipticalArchShape.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

EllipticalArchShape::EllipticalArchShape(double span, double outer_rise, double inner_rise)
    : span_(span), h_outer_(outer_rise), h_inner_(inner_rise) {}

double EllipticalArchShape::outerY(double x) const {
    double a = span_ / 2.0;
    double X = x / a;
    if (std::abs(X) > 1.0) return 0.0;
    return h_outer_ * std::sqrt(1.0 - X * X);
}

double EllipticalArchShape::innerY(double x) const {
    double a = span_ / 2.0;
    double X = x / a;
    if (std::abs(X) > 1.0) return 0.0;
    return h_inner_ * std::sqrt(1.0 - X * X);
}

bool EllipticalArchShape::isInside(double x, double y) const {
    double top = outerY(x);
    double bottom = innerY(x);
    return y >= bottom && y <= top;
}

void EllipticalArchShape::generateArchNodes(Model<2>& model, int num_arch_nodes, int num_vertical_layers) const {
    double x_min = -span_ / 2.0;
    double x_max =  span_ / 2.0;

    for (int i = 0; i < num_arch_nodes; ++i) {
        double x = x_min + (x_max - x_min) * i / (num_arch_nodes - 1);

        double y_outer = outerY(x);
        double y_inner = innerY(x);

        if (std::isnan(y_outer) || std::isnan(y_inner)) continue;

        for (int j = 0; j < num_vertical_layers; ++j) {
            double t = (num_vertical_layers == 1) ? 0.0 : static_cast<double>(j) / (num_vertical_layers - 1);
            double y = y_inner + t * (y_outer - y_inner);

            size_t node_id = model.addNode({x, y});
            std::cout << "[DEBUG] Added node " << node_id << " at (" << x << ", " << y << ")\n";
        }
    }
}
