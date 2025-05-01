


#include "Model.hpp"

size_t Model::addNode(double x, double y, bool fixed_x, bool fixed_y, double fx, double fy) {
    x_coords.push_back(x);
    y_coords.push_back(y);
    support_x.push_back(fixed_x);
    support_y.push_back(fixed_y);
    load_x.push_back(fx);
    load_y.push_back(fy);
    return x_coords.size() - 1;
}