#pragma once

#include <string>
#include "../model/TrussStructure.hpp"
#include "../model/OptimizationResults.hpp" 
/**
 * @class OutputExporter
 * @brief Handles exporting optimization results to various output formats.
 *
 * This class provides utility functions to export the results of
 * a truss optimization to SVG, CSV, and JSON formats.
 */
class OutputExporter {
public:
    OutputExporter() = default;
    /**
     * @brief Exports the optimized truss structure as an SVG image.
     * 
     * @param truss The truss structure.
     * @param results The optimization results.
     * @param filepath The file path to write the SVG to.
     * @param scale_by_area Whether to scale line thickness by cross-sectional area.
     */
    void exportToSVG(const TrussStructure<2>& truss, const OptimizationResults& results, const std::string& filepath, bool scale_by_area);
    /**
     * @brief Exports the optimization results to a CSV file.
     * 
     * @param results The optimization results.
     * @param filepath The file path to write the CSV to.
     */
    void exportToCSV(const OptimizationResults& results, const std::string& filepath);
    /**
     * @brief Exports the optimization results to a JSON file.
     * 
     * @param results The optimization results.
     * @param filepath The file path to write the JSON to.
     */
    void exportToJSON(const OptimizationResults& results, const std::string& filepath);
    /**
     * @brief Prints a human-readable summary of the optimization results to the console.
     * 
     * @param results The optimization results.
     */
    static void printToConsole(const OptimizationResults& results);
};
