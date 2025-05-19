#include "Model.hpp"

template <int Dim>
size_t Model<Dim>::addNode(const std::array<double, Dim>& pos) {
    for (int d = 0; d < Dim; ++d) {
        coords[d].push_back(pos[d]);
        support[d].push_back(false);
        load[d].push_back(0.0);
    }
    return coords[0].size() - 1;
}

template <int Dim>
void Model<Dim>::fixNode(size_t id, const std::array<bool, Dim>& fix) {
    for (int d = 0; d < Dim; ++d)
        support[d][id] = fix[d];
}

template <int Dim>
void Model<Dim>::setLoad(size_t id, const std::array<double, Dim>& f) {
    for (int d = 0; d < Dim; ++d)
        load[d][id] = f[d];
    std::cout << "[DEBUG] Load on node " << id << ": ";
    for (int d = 0; d < Dim; ++d)
        std::cout << "f" << d << "=" << f[d] << " ";
    std::cout << "\n";
}

template <int Dim>
size_t Model<Dim>::nodeCount() const {
    return coords[0].size();
}

template <int Dim>
double Model<Dim>::getCoord(size_t i, int d) const {
    return coords[d][i];
}

template <int Dim>
bool Model<Dim>::isSupport(size_t i, int d) const {
    return support[d][i];
}

template <int Dim>
double Model<Dim>::getLoad(size_t i, int d) const {
    return load[d][i];
}

// Explicit instantiations
template class Model<2>;