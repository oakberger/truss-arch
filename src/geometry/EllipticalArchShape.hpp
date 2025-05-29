#pragma once

#include "model/Model.hpp"
#include "BaseArchShape.hpp"

/**
 * @brief Represents a 2D elliptical arch with variable thickness.
 * 
 * The outer and inner curves are scaled parts of the same ellipse,
 * each defined by a separate rise height. This allows for a variable 
 * band thickness along the arch.
 */
class EllipticalArchShape : public BaseArchShape {
public:
    /**
     * @param span Total horizontal length of the arch
     * @param outer_rise Height of the outer curve (top of the arch)
     * @param inner_rise Height of the inner curve (bottom of the arch)
     */
    EllipticalArchShape(double span, double outer_rise, double inner_rise);
    double outerY(double x) const override;
    double innerY(double x) const override;
    bool isInside(double x, double y) const override;
    void generateArchNodes(Model<2>& model, int num_arch_nodes, int num_vertical_layers) const override;

private:
    double span_;
    double h_outer_;
    double h_inner_;
};

