#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include <string>
#include "../core/TrussStructure.hpp"

/**
 * @class InputParser
 * @brief Handles reading and parsing of structural input files.
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

#endif // INPUT_PARSER_HPP
