#include "OptimizationResults.hpp"
#include <cmath>

void OptimizationResults::addEntry(int bar_id, double force, double length, double area) {
    bar_forces.push_back({bar_id, force, length, area, false});
}

void OptimizationResults::computeTotalVolume() {
    total_volume = 0.0;
    for (const auto& entry : bar_forces) {
        total_volume += std::abs(entry.force) * entry.length; //Volume (Optimization criteria) can be changed
    }
}

void OptimizationResults::markActiveBars(double tolerance) {
    for (auto& entry : bar_forces) {
        entry.is_active = std::abs(entry.force) > tolerance;
    }
}