#pragma once

#include "model/Model.hpp"
#include "BaseArchShape.hpp"

class EllipticalArchShape : public BaseArchShape {
public:
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

