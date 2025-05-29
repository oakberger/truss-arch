#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <cassert>
#include "../geometry/BaseArchShape.hpp"
#include "Model.hpp"
#include "Bar.hpp"

/**
 * @struct MaterialProperties
 * @brief Holds material properties for the truss structure.
 * 
 * This structure contains yield stress values for both tension and compression
 */
struct MaterialProperties {
    double sigma_tension = 1.0;     // Yield stress in tension
    double sigma_compression = 1.0; // Yield stress in compression
};

/**
 * @brief Central data model of the truss arch optimizer.
 * 
 * Templated by spatial dimension (e.g., Dim = 2 or 3).
 * Responsibilities:
 * - Store nodes and bars
 * - Generate node grid
 * - Generate ground structure (bar connections)
 * - Assign loads and supports
 */
template <int Dim>
class TrussStructure {
public:
    Model<Dim> model;                   ///< Struct-of-arrays storage for node data
    std::vector<Bar> bars;             ///< All bars (candidate members)
    MaterialProperties material;       ///< Material data for yield stress

    /**
     * @brief Adds a node at the given position to the truss model.
     * @param pos The spatial coordinates of the node (array of size Dim)
     * @return The index of the newly added node
     */
    size_t addNode(const std::array<double, Dim>& pos);
    /**
     * @brief Adds a bar (candidate truss member) between two nodes.
     * @param start_id Index of the starting node
     * @param end_id Index of the ending node
     */
    void addBar(std::size_t start_id, std::size_t end_id);
    /**
     * @brief Generates a 2D grid of nodes with uniform spacing.
     * @param rows Number of vertical grid points
     * @param cols Number of horizontal grid points
     * @param spacing Distance between adjacent nodes
     */
    void generateGrid(int rows, int cols, double spacing);

    /**
     * @brief Creates bars between all node pairs within a maximum length.
     * 
     * This forms the full ground structure (all-to-all connections),
     * filtered by a length constraint.
     * 
     * @param max_length Maximum length allowed between any two connected nodes
     */
    void generateGroundStructure(double max_length);

    /**
     * @brief Creates bars within an arch-constrained region.
     * 
     * Filters node connections by max length and verifies that the full bar
     * (including midpoint) lies inside the defined arch shape.
     * 
     * @param max_length Maximum bar length
     * @param arch Reference to a shape object used to restrict bars
     */
    void generateGroundStructureArch(double max_length, const BaseArchShape& arch);
};
