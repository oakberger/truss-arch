#include "Bar.hpp"
#include "Model.hpp"
#include <cmath>

Bar::Bar(int id_, int start, int end, double length_)
    : id(id_), node_start(start), node_end(end), length(length_),
      area(0.0), force(0.0), is_active(false) {}

      std::pair<double, double> Bar::direction(const Model<2>& model) const {
        std::size_t i = static_cast<std::size_t>(node_start);
        std::size_t j = static_cast<std::size_t>(node_end);
    
        double dx = model.coords[0][j] - model.coords[0][i];
        double dy = model.coords[1][j] - model.coords[1][i];
        double len = std::sqrt(dx * dx + dy * dy);
    
        if (len == 0.0) return {0.0, 0.0};  // avoid division by zero
        return {dx / len, dy / len};
    }
