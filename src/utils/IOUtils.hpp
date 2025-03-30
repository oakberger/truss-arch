#pragma once
#include "../model/TrussStructure.hpp"
#include <string>

/**
 * @brief Utility class for exporting and debugging output.
 * 
 * Responsibilities:
 * - Save results to SVG, CSV, or JSON
 * - Possibly load configs or support files
 * - Visualize or debug the truss structure
 * - Interface with external visualization tools
 */
class IOUtils {
public:
    static void exportToSVG(const TrussStructure& structure, const std::vector<bool>& active_bars, const std::string& filename);
};