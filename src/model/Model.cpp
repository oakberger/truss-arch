#include "Model.hpp"
#include <iostream>

size_t Model::addNode(double x, double y) {
    x_coords.push_back(x);
    y_coords.push_back(y);
    support_x.push_back(false);
    support_y.push_back(false);
    load_x.push_back(0.0);
    load_y.push_back(0.0);
    return x_coords.size() - 1;
}

void Model::fixNode(size_t id, bool fix_x, bool fix_y) {
    support_x[id] = fix_x;
    support_y[id] = fix_y;
}

void Model::setLoad(size_t id, double fx, double fy) {
    load_x[id] = fx;
    load_y[id] = fy;
    std::cout << "[DEBUG] Load applied to node " << id << ": fx=" << fx << ", fy=" << fy << "\n";

}