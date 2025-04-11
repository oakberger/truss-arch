#pragma once

/**
 * @brief Represents a node in the truss structure.
 * 
 * A Node is a point in space where truss bars connect. It is necessary for:
 * - Generating the equilibrium matrix
 * - Visualizing the structure
 * - Defining supports and external loads
 * 
 * Each node must have:
 * - A unique ID
 * - Position (x, y)
 * - Support conditions (fixed in x/y directions)
 * - External loads (fx, fy)
 */
struct Node {
    int id;               ///< Unique identifier
    double x, y;          ///< Coordinates
    bool is_support_x;    ///< Support in x-direction
    bool is_support_y;    ///< Support in y-direction
    double fx = 0.0;      ///< External force in x-direction
    double fy = 0.0;      ///< External force in y-direction

    /**
     * @brief Constructs a node with given ID and coordinates.
     * Support conditions and loads are set later.
     */
    Node(int id_, double x_, double y_);
};