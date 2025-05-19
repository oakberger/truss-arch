#pragma once
#include <vector>
#include <array>
#include <cstddef>
#include <iostream>

// Generic Model class for any dimension (2D, 3D, etc.)
template <int Dim>
class Model {
public:
    std::array<std::vector<double>, Dim> coords;
    std::array<std::vector<bool>, Dim> support;
    std::array<std::vector<double>, Dim> load;

    size_t addNode(const std::array<double, Dim>& pos);
    void fixNode(size_t id, const std::array<bool, Dim>& fix);
    void setLoad(size_t id, const std::array<double, Dim>& f);

    size_t nodeCount() const;
    double getCoord(size_t i, int d) const;
    bool isSupport(size_t i, int d) const;
    double getLoad(size_t i, int d) const;
};