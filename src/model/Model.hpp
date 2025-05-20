#pragma once
#include <vector>
#include <array>
#include <cstddef>
#include <iostream>

/**
 * @class Model
 * @brief Represents a geometric model with nodes, supports, and loads in arbitrary dimensions (only supports 2d at the moment).
 *
 * This generic class stores nodal positions, boundary conditions, and external loads
 * for structural models in 2D, 3D, etc., depending on the template parameter `Dim`.
 * 
 * @tparam Dim The number of spatial dimensions (e.g., 2 for 2D, 3 for 3D).
 */
template <int Dim>
class Model {
public:
    /// Nodal coordinates for each dimension.
    std::array<std::vector<double>, Dim> coords;

    /// Support conditions per DOF (true = fixed).
    std::array<std::vector<bool>, Dim> support;

    /// Applied loads per DOF.
    std::array<std::vector<double>, Dim> load;

    /**
     * @brief Adds a new node at the specified position.
     * @param pos Array of coordinates (length = Dim).
     * @return Index of the newly added node.
     */
    size_t addNode(const std::array<double, Dim>& pos);

    /**
     * @brief Sets the support condition for a node.
     * @param id Node index.
     * @param fix Array indicating fixed directions (true = fixed).
     */
    void fixNode(size_t id, const std::array<bool, Dim>& fix);

    /**
     * @brief Applies an external load to a node.
     * @param id Node index.
     * @param f Array of force components.
     */
    void setLoad(size_t id, const std::array<double, Dim>& f);

    /**
     * @brief Returns the number of nodes in the model.
     */
    size_t nodeCount() const;

    /**
     * @brief Returns the coordinate value of node `i` in dimension `d`.
     */
    double getCoord(size_t i, int d) const;

    /**
     * @brief Returns whether node `i` is supported in dimension `d`.
     */
    bool isSupport(size_t i, int d) const;

    /**
     * @brief Returns the applied load on node `i` in dimension `d`.
     */
    double getLoad(size_t i, int d) const;
};