#pragma once

#include "BaseArchShape.hpp"

/**
 * @brief Represents a 2D parabolic arch with thickness (arch band).
 *
 * Defines both an outer and inner boundary, so the truss remains
 * physically inside the arch cross-section. 
 * Geometry can be changed later on if needed.
 * The shape of the arch could also be calculated seperately and optimized.
 */
class ParallelArchShape : public BaseArchShape {
public:
    /**
     * @param span Total horizontal length of the arch
     * @param rise Maximum height of the arch (outer curve)
     * @param thickness Vertical offset between outer and inner arch
     */
    ParallelArchShape(double span, double rise, double thickness);

    double outerY(double x) const override;
    double innerY(double x) const override;
    bool isInside(double x, double y) const override;

    void generateArchNodes(Model& model,
                           int num_arch_nodes,
                           int num_vertical_layers) const override;

private:
    double span_, rise_, thickness_;
};