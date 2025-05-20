#pragma once
#include "../model/Model.hpp"
/**
 * @brief Abstract base class for 2D arch shapes.
 * 
 * This interface defines the geometric profile of a truss arch structure,
 * including its outer and inner boundaries. Concrete subclasses define
 * specific arch geometries (e.g. parabolic, elliptical).
 */
class BaseArchShape {
public:
    virtual ~BaseArchShape() = default;
    /**
     * @brief Computes the upper (outer) y-boundary of the arch at a given x.
     * @param x Horizontal coordinate.
     * @return y-coordinate of the upper arch boundary.
     */
    virtual double outerY(double x) const = 0;
    /**
     * @brief Computes the lower (inner) y-boundary of the arch at a given x.
     * @param x Horizontal coordinate.
     * @return y-coordinate of the lower arch boundary.
     */
    virtual double innerY(double x) const = 0;
    /**
     * @brief Checks whether a point lies inside the arch region.
     * @param x Horizontal coordinate.
     * @param y Vertical coordinate.
     * @return True if the point is within the arch bounds, false otherwise.
     */
    virtual bool isInside(double x, double y) const = 0;
    /**
     * @brief Populates the model with nodes along the arch domain.
     * 
     * Nodes are generated along the arch span and within vertical layers.
     * 
     * @param model The model object to populate with nodes.
     * @param num_arch_nodes Number of nodes along the horizontal span of the arch.
     * @param nuvertical_layers Number of vertical layers of nodes inside the arch.
     */
    virtual void generateArchNodes(Model<2>& model,
                                    int num_arch_nodes,
                                    int nuvertical_layers) const = 0;
};
