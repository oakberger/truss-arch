#ifndef OPTIMIZATION_RESULTS_HPP
#define OPTIMIZATION_RESULTS_HPP

#include <vector>

/**
 * @brief Holds data about a single optimized bar.
 *
 * Each BarForceEntry stores the result for one bar in the structure,
 * including its internal force, length, and whether it's active
 * (i.e., significantly used in the final solution).
 */
struct BarForceEntry {
    int bar_id;        ///< ID of the bar
    double force;      ///< Internal force in the bar (positive = tension, negative = compression)
    double length;     ///< Length of the bar
    double area;       ///< Cross-sectional area of the bar
    bool is_active;    ///< Whether this bar is used in the final optimized structure
};

/**
 * @brief Collects the full result of the optimization process.
 *
 * After solving the truss optimization problem, an instance of this class
 * stores all relevant output: forces, volume, and status of each bar.
 * It is returned by the optimizer (e.g., LPOptimizer) and can be passed to
 * exporters for visualization or file output.
 */
class OptimizationResults {
    public:
        OptimizationResults() = default;
    
        std::vector<BarForceEntry> bar_forces;  ///< List of all bar result entries
        double total_volume = 0.0;              ///< Total volume of active bars
        double objective_value = 0.0;           ///< Final value of the optimization objective
    
        /**
         * @brief Adds a new entry to the results list.
         */
        void addEntry(int bar_id, double force, double length, double area);
    
        /**
         * @brief Computes total physical volume assuming bar stress limit is used.
         *
         * Each bar is assumed to operate exactly at the stress limit (σ_t or σ_c),
         * such that area = |F| / σ. Then volume = A * L = |F| * L / σ.
         * 
         * @param sigma_tension Maximum allowable tensile stress.
         * @param sigma_compression Maximum allowable compressive stress.
         */
        void computeTotalVolume(double sigma_tension, double sigma_compression);
    
        /**
         * @brief Marks bars as active if their force exceeds a given threshold.
         * @param tolerance Minimum force magnitude to consider a bar active (default: 1e-6)
         */
        void markActiveBars(double tolerance = 1e-6);
    };
    
    #endif // OPTIMIZATION_RESULTS_HPP