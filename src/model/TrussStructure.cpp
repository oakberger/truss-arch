#include "TrussStructure.hpp"
#include <cmath>
#include <cassert>

template <int Dim>
size_t TrussStructure<Dim>::addNode(const std::array<double, Dim>& pos) {
    return model.addNode(pos);
}

template <int Dim>
void TrussStructure<Dim>::addBar(std::size_t start_id, std::size_t end_id) {
    assert(start_id >= 0 && end_id >= 0);

    double len_sq = 0.0;
    for (int d = 0; d < Dim; ++d) {
        double diff = model.coords[d][end_id] - model.coords[d][start_id];
        len_sq += diff * diff;
    }
    double len = std::sqrt(len_sq);

    bars.emplace_back(bars.size(), static_cast<int>(start_id), static_cast<int>(end_id), len);
}

template <int Dim>
void TrussStructure<Dim>::generateGrid(int rows, int cols, double spacing) {
    static_assert(Dim == 2, "generateGrid only implemented for 2D.");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            addNode({j * spacing, i * spacing});
        }
    }
}

template <int Dim>
void TrussStructure<Dim>::generateGroundStructure(double max_length) {
    size_t N = model.nodeCount();
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            double dist2 = 0.0;
            for (int d = 0; d < Dim; ++d) {
                double diff = model.coords[d][j] - model.coords[d][i];
                dist2 += diff * diff;
            }
            if (std::sqrt(dist2) <= max_length) {
                addBar(i, j);
            }
        }
    }
}

template <int Dim>
void TrussStructure<Dim>::generateGroundStructureArch(double max_length, const BaseArchShape& arch) {
    static_assert(Dim == 2, "generateGroundStructureArch only implemented for 2D.");
    size_t N = model.nodeCount();
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            double xi = model.coords[0][i];
            double yi = model.coords[1][i];
            double xj = model.coords[0][j];
            double yj = model.coords[1][j];

            double mx = 0.5 * (xi + xj);
            double my = 0.5 * (yi + yj);

            double dx = xj - xi;
            double dy = yj - yi;
            double dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= max_length &&
                arch.isInside(xi, yi) &&
                arch.isInside(xj, yj) &&
                arch.isInside(mx, my)) {
                addBar(i, j);
            }
        }
    }
}

// Explicit instantiation
template class TrussStructure<2>;
