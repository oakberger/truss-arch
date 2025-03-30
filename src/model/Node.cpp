#include "Node.hpp"

Node::Node(int id_, double x_, double y_, bool support_x, bool support_y)
    : id(id_), x(x_), y(y_), is_support_x(support_x), is_support_y(support_y) {}