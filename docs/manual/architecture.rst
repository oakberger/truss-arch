=====================
Architecture Overview
=====================

This document gives an overview of how I’ve structured the **Truss Arch Optimizer**, a C++ project where I try to implement the **Ground Structure Method** for optimizing truss layouts of archs. The goal is to find the best way to arrange bars in an arch-like structure to use as little material as possible, while still holding up the loads.

I tried to structure the code so that it’s easy to understand and extend.
-----------
Core Idea
-----------

The core of the code is a ``TrussStructure`` class that stores the model: nodes and bars. Around that, I’ve added components for optimization, matrix building, and (eventually) visualization.

----------------
Class Overview
----------------

Node
====

A ``Node`` is a point in the structure where bars can connect. It has:

- An ``(x, y)`` position
- Information about whether it's supported in ``x`` and/or ``y``
- Optionally a force applied in ``x`` or ``y``

Bar
====

A ``Bar`` is a possible member between two nodes. It has:

- A start and end node index
- Its geometric length (calculated from node positions)
- A cross-sectional area and force (these are the design variables)
- A boolean to keep track of whether this bar is currently active in the structure

TrussStructure
==============

This class holds everything — all nodes and bars — and has methods to:

- Add nodes and bars manually
- Create a regular grid of nodes
- Connect nodes to each other to form the ground structure (only if they’re within a certain distance)

Right now, the bars are just connected based on distance, but I will add more rules later to keep things inside a given arch shape or exclude weird angles.

---------------------
Optimization Setup
---------------------

I created a base class called ``BaseOptimizer``, which defines a common interface for solvers. This makes it easier to try different approaches without changing too much code.

BaseOptimizer
=============

This is an abstract class that all optimizers should inherit from. It requires:

- A ``solve()`` function that takes in the structure and solves the problem
- An ``exportResults()`` function to output the final layout

LPOptimizer
===========

This is the first solver. It uses **linear programming** to minimize the total volume of the structure by adjusting the cross-sectional areas of the bars.

---------------
MatrixBuilder
---------------

This class will handle creating the system of equations:

.. math::

   A \cdot f = b

where:

- ``A`` is the equilibrium matrix (built from the bar directions)
- ``f`` is the vector of bar forces
- ``b`` is the vector of external loads on each node

---------
IOUtils
---------

This is a helper class for exporting results and debugging. It has a placeholder for writing SVG files to visualize the final truss. The plan is to eventua also output CSV or JSON, or maybe even read in support/load configs.

----------
main.cpp
----------

This file ties everything together. It:

- Creates a grid of nodes
- Connects nearby nodes to form a ground structure
- Adds supports and load
- Runs the LP optimizer
- Exports the result

-------------
Future Stuff
-------------

- I’ve left placeholders for a ``MILPOptimizer`` and ``SolverInterface``, which I might use if I want to add binary variables or more complex optimization later.
- ``IOUtils`` could be expanded to show real-time output or integrate with Python/Matplotlib.
