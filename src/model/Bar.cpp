#include "Bar.hpp"
#include "Model.hpp"
#include <cmath>

Bar::Bar(int id_, int start, int end, double length_)
    : id(id_), node_start(start), node_end(end), length(length_),
      area(0.0), force(0.0), is_active(false) {}

std::pair<double, double> Bar::direction(const Model& model) const {
    double dx = model.x_coords[node_end] - model.x_coords[node_start];
    double dy = model.y_coords[node_end] - model.y_coords[node_start];
    double len = std::sqrt(dx * dx + dy * dy);
    if (len == 0.0) return {0.0, 0.0};  // avoid division by zero
    return {dx / len, dy / len};
}
