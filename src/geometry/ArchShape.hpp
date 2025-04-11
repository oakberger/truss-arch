#pragma once

#include "../model/Node.hpp"
#include <vector>

/**
 * @brief Represents a 2D parabolic arch with thickness (arch band).
 *
 * Defines both an outer and inner boundary, so the truss remains
 * physically inside the arch cross-section.
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

    /// Outer arch y-value at x
    double outerY(double x) const;

    /// Inner arch y-value at x
    double innerY(double x) const;

    /// Whether a point is inside the arch band
    bool isInside(double x, double y) const;

    /// Generate nodes inside the arch band
    std::vector<Node> generateArchNodes(int nx, int ny, double spacing) const;

private:
    double span_;
    double rise_;
    double thickness_;
};
