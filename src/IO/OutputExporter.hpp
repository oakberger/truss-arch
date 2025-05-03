#pragma once

#include <string>
#include "../model/TrussStructure.hpp"
#include "../model/OptimizationResults.hpp" 

/**
 * @class OutputExporter
 * @brief Handles exporting of truss structures and results.
 *
 * Takes TrussStructure and OptimizationResults and exports the results.
 * The results can then be used for:
 * - Post-processing (If the data needs to be filtered or modified)
 * - Visualization (maybe with Python)
 */

class OutputExporter {
public:
    OutputExporter() = default;

    void exportToSVG(const TrussStructure& truss, const std::string& filepath);
    void exportToCSV(const OptimizationResults& results, const std::string& filepath);
    void exportToJSON(const OptimizationResults& results, const std::string& filepath);
    static void printToConsole(const OptimizationResults& results);
};
