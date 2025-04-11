#pragma once

#include <string>
#include "../model/TrussStructure.hpp"

/**
 * @class InputParser
 * @brief Handles reading and parsing of structural input files.
 *
 * Not Sure if InputParser will be needed in the end. But will leave it for now,
 * as it allows the code to be easily extended for additional functionality. 
 * 
 * This class is responsible for constructing a TrussStructure
 * from external files such as JSON. It encapsulates all input-related logic.
 */

class InputParser {
public:
    InputParser() = default;

    // Parses a structure from a JSON file
    TrussStructure parseFromJSON(const std::string& filepath);

    // Optionally: parse from CSV or custom formats
    // TrussStructure parseFromCSV(const std::string& filepath);
};

