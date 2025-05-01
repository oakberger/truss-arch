#pragma once
#include <vector>
#include "../geometry/ArchShape.hpp"
#include "Model.hpp"
#include "Bar.hpp"

/**
 * @brief Central data model of the truss arch optimizer.
 * 
 * The TrussStructure contains all structural information needed by the optimizer and matrix builder.
 * Responsibilities:
 * - Store nodes and bars
 * - Generate node grid
 * - Generate ground structure (bar connections)
 * - Assign loads and supports
 */
class TrussStructure {
public:
    Model model;              ///< Struct-of-arrays storage for node data
    std::vector<Bar> bars;    ///< All bars (candidate members)

    void addNode(double x, double y);
    void addBar(int start_id, int end_id);
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
    void generateGroundStructureArch(double max_length, const ArchShape& arch);

    // Removed getNode() accessor because Model does not use individual Node objects
};