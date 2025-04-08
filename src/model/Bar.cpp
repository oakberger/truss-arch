#include "Bar.hpp"

Bar::Bar(int id_, int start, int end, double length_)
    : id(id_), node_start(start), node_end(end), length(length_),
      area(0.0), force(0.0), is_active(false) {}