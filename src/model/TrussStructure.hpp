#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <cassert>
#include "../geometry/BaseArchShape.hpp"
#include "Model.hpp"
#include "Bar.hpp"

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
struct MaterialProperties {
    double sigma_tension = 1.0;     // Yield stress in tension
    double sigma_compression = 1.0; // Yield stress in compression
};

template <int Dim>
class TrussStructure {
public:
    Model<Dim> model;                   ///< Struct-of-arrays storage for node data
    std::vector<Bar> bars;             ///< All bars (candidate members)
    MaterialProperties material;       ///< Material data for yield stress

    size_t addNode(const std::array<double, Dim>& pos);
    void addBar(std::size_t start_id, std::size_t end_id);
    void generateGrid(int rows, int cols, double spacing);

    /**
     * @brief Generates the ground structure, constrained with maximum length
     * @param max_length Maximum length allowed between nodes
     */
    void generateGroundStructure(double max_length);

    /**
     * @brief Generates the ground structure, constrained within an arch region.
     * @param max_length Maximum length allowed between nodes
     * @param arch Shape to constrain the bars inside
     */
    void generateGroundStructureArch(double max_length, const BaseArchShape& arch);
};
