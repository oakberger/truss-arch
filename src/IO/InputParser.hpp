#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "../model/TrussStructure.hpp"
#include "../geometry/BaseArchShape.hpp"
#include "../geometry/ParallelArchShape.hpp"
#include "../geometry/EllipticalArchShape.hpp"

/**
 * @file InputParser.hpp
 * @brief Parses structural input data from JSON.
 *
 * This class reads input from a JSON file to construct a TrussStructure
 * for optimization. Two geometric input modes are supported: a rectangular grid
 * or an arch region.
 *
 * ### JSON Input Format
 *
 * The root JSON object must contain one of the following sections:
 * - `"rectangle"`: Defines a rectangular node grid.
 * - `"arch"`: Defines a elliptical or parallel arch domain.
 *
 * Additionally, the following sections are required:
 * - `"material"`: Contains the allowable stress values.
 * - `"boundary_conditions"`: Lists supports and external loads.
 * - `"output"`: Specifies output file paths for results.
 *
 * ### Rectangular Grid Example
 * @code{.json}
 * {
 *   "rectangle": {
 *     "grid_x": 29,
 *     "grid_y": 20,
 *     "spacing": 1.0,
 *     "max_bar_length": 4.0
 *   },
 *   "material": {
 *     "sigma_tension": 1.0,
 *     "sigma_compression": 1.0
 *   },
 *   "boundary_conditions": {
 *     "supports": [
 *       { "node_id": 0, "x": true, "y": true },
 *       { "node_id": 28, "x": false, "y": true }
 *     ],
 *     "loads": [
 *       { "node_id": 565, "fx": 0.0, "fy": -100.0 }
 *     ]
 *   },
 *   "output": {
 *     "svg": "output.svg",
 *     "csv": "results.csv",
 *     "json": "results.json"
 *   }
 * }
 * @endcode
 *
 * ### Parallel Arch Example
 * @code{.json}
 * {
 *   "arch": {
 *     "type": "parallel",
 *     "span": 10.0,
 *     "rise": 3.0,
 *     "thickness": 0.5,
 *     "num_arch_nodes": 21,
 *     "num_vertical_layers": 3,
 *     "max_bar_length": 5.0
 *   },
 *   "material": {
 *     "sigma_tension": 250.0,
 *     "sigma_compression": 250.0
 *   },
 *   "boundary_conditions": {
 *     "supports": [
 *       { "node_id": 0, "x": true, "y": true },
 *       { "node_id": 60, "x": false, "y": true }
 *     ],
 *     "loads": [
 *       { "node_id": 32, "fx": 0.0, "fy": -100.0 }
 *     ]
 *   },
 *   "output": {
 *     "svg_force": "arch_force.svg",
 *     "svg_area": "arch_area.svg",
 *     "csv": "results.csv",
 *     "json": "results.json"
 *   }
 * }
 * @endcode
 *
 *  ### Elliptical Arch Example
 * @code{.json}
 *{
 *"arch": {
 * "type": "elliptical",
 *"span": 200,
 * "outer_rise": 80.0,
 * "inner_rise": 30.0,
 * "num_arch_nodes": 51,
 * "num_vertical_layers": 6,
 * "max_bar_length": 15.0
 *},
 * "material": {
 *   "sigma_tension": 250.0,
 *   "sigma_compression": 250.0
 * },
 * "boundary_conditions": {
 *   "supports": [
 *     { "node_id": 0, "x": true, "y": true },
 *     { "node_id": 300, "x": true, "y": true }
 *   ],
 *   "loads": [
 *     { "node_id": 155, "fx": 0.0, "fy": -1000.0 }
 *   ]
 * },
 * "output": {
 *   "svg_force": "examples/output/elliptical_arch_1_force.svg",
 *   "svg_area": "examples/output/elliptical_arch_1_area.svg",
 *   "csv": "examples/output/elliptical_arch_1.csv",
 *   "json": "examples/output/elliptical_arch_1.json"
 * }
 *}
 * @endcode
 * 
 * @note Node IDs in boundary conditions (loads and supports) must match those generated during node grid creation.
 */

/**
 * @class InputParser
 * @brief Responsible for parsing input data into a TrussStructure.
 * 
 * Check file documentation for more info on how the input works.
 */

class InputParser {
public:
    InputParser() = default;

      /**
     * @brief Parses a 2D truss structure from a JSON object.
     * 
     * The JSON is expected to define nodes, bars, material properties,
     * supports, and loads in a structured format.
     * 
     * @param j JSON object containing the structure data.
     * @return A constructed TrussStructure<2> instance.
     */
    TrussStructure<2> parseFromJSON(const nlohmann::json& j);
};
