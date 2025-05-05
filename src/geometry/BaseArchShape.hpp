#pragma once
#include "../model/Model.hpp"

class BaseArchShape {
public:
    virtual ~BaseArchShape() = default;

    virtual double outerY(double x) const = 0;
    virtual double innerY(double x) const = 0;
    virtual bool isInside(double x, double y) const = 0;

    virtual void generateArchNodes(Model& model,
                                    int num_arch_nodes,
                                    int num_vertical_layers) const = 0;
};
