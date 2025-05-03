#pragma once
#include <vector>

class Model {
public:
    // Nodal coordinates
    std::vector<double> x_coords;
    std::vector<double> y_coords;

    // Support conditions (true = fixed)
    std::vector<bool> support_x;
    std::vector<bool> support_y;

    // External loads
    std::vector<double> load_x;
    std::vector<double> load_y;

    // Add a node and return its index
    size_t addNode(double x, double y);

    void fixNode(size_t id, bool fix_x, bool fix_y);
    void setLoad(size_t id, double fx, double fy);

    // Number of nodes
    size_t nodeCount() const {
        return x_coords.size();
    }

    // Accessors
    double getX(size_t i) const { return x_coords[i]; }
    double getY(size_t i) const { return y_coords[i]; }
    bool isSupportX(size_t i) const { return support_x[i]; }
    bool isSupportY(size_t i) const { return support_y[i]; }
    double getLoadX(size_t i) const { return load_x[i]; }
    double getLoadY(size_t i) const { return load_y[i]; }
};