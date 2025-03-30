#pragma once
#include <vector>
#include "Node.hpp"
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
    std::vector<Node> nodes;  ///< All nodes in the structure
    std::vector<Bar> bars;    ///< All bars (candidate members)

    void addNode(double x, double y, bool support_x = false, bool support_y = false);
    void addBar(int start_id, int end_id);
    void generateGrid(int rows, int cols, double spacing);
    void generateGroundStructure(double max_length);
};