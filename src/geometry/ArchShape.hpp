#pragma once

#include "../model/Model.hpp"
#include <vector>

/**
 * @brief Represents a 2D parabolic arch with thickness (arch band).
 *
 * Defines both an outer and inner boundary, so the truss remains
 * physically inside the arch cross-section. 
 * Geometry can be changed later on if needed.
 * The shape of the arch could also be calculated seperately and optimized.
 */
class ArchShape {
public:
    ArchShape() = default;

    /**
     * @param span Total horizontal length of the arch
     * @param rise Maximum height of the arch (outer curve)
     * @param thickness Vertical offset between outer and inner arch
     */
    ArchShape(double span, double rise, double thickness);

    void generateArchNodes(Model& model, int nx, int ny, double spacing) const;

    /// Outer arch y-value at x
    double outerY(double x) const;

    /// Inner arch y-value at x
    double innerY(double x) const;

    /// Whether a point is inside the arch band
    bool isInside(double x, double y) const;

    /// Generate nodes inside the arch band

private:
    double span_;
    double rise_;
    double thickness_;
};