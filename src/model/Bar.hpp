#pragma once

/**
 * @brief Represents a truss bar (member) connecting two nodes.
 * 
 * Bars define candidate members in the ground structure. They are used for:
 * - Calculating force directions in the equilibrium matrix
 * - Possibly carrying force or design variables (e.g., area)
 * 
 * Each bar includes:
 * - A unique ID
 * - Start and end node indices
 * - Geometric length
 */
struct Bar {
    int id;              ///< Unique identifier
    int node_start;      ///< Start node index
    int node_end;        ///< End node index
    double length;       ///< Length of the bar

    Bar(int id_, int start, int end, double length_);
};