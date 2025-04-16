
######################
Architecture Overview
######################

This document gives an overview of how the **Truss Arch Optimizer** is structured, a C++ project implementing the **Ground Structure Method (GSM)** for optimizing truss layouts constrained within arch-like regions. The goal is to find efficient, material-saving truss layouts that still satisfy mechanical equilibrium and support conditions.

The code is structured to prioritize clarity, modularity, and extensibility.

######################
Core Idea
######################

The core class is ``TrussStructure``, which stores the entire structural model — nodes and bars — and handles geometry generation. Around it are components for:

- Optimization
- Matrix building
- Arch shape handling
- Input/output for JSON, SVG, CSV

The architecture is designed to allow future features.

######################
Class Overview
######################

-----------------
Node
-----------------

A ``Node`` represents a point in 2D space. Each node has:

- A unique ``id``
- ``x, y`` coordinates
- Boolean flags for supports in ``x`` and/or ``y``
- External loads ``fx, fy``, initialized to 0

Nodes are created with geometry only (id, x, y), and supports or loads are applied separately. This clean separation improves flexibility and allows different loading scenarios on the same geometry.

-----------------
Bar
-----------------

A ``Bar`` is a possible connection between two nodes. It includes:

- Start and end node indices
- Geometric length (auto-computed from nodes)
- Design variables: force and cross-sectional area
- A flag ``is_active`` indicating whether the bar is used in the final layout

-----------------
TrussStructure
-----------------

The ``TrussStructure`` class acts as the full container for the model. It manages:

- A list of all ``Node`` and ``Bar`` objects
- Methods to add nodes and bars manually
- Automated creation of a node grid (``generateGrid``)
- Construction of the ground structure by connecting nodes (``generateGroundStructure``)

A specialized method ``generateGroundStructureArch`` constrains bar generation to lie **within a defined arch-shaped band**, leveraging the ``ArchShape`` class.

######################
Arch Geometry
######################

-----------------
ArchShape
-----------------

``ArchShape`` defines a 2D parabolic arch region with a given span, rise, and thickness. It’s used to:

- Generate nodes inside the arch band (using ``generateArchNodes``)
- Validate whether a node or bar lies within the arch (using ``isInside``)

This is crucial for keeping the truss layout realistic and arch-conforming.

######################
Optimization Setup
######################

-----------------
BaseOptimizer
-----------------

An abstract base class that defines the interface for all solvers. It provides:

- ``solve(structure)`` to compute optimized cross-sectional areas
- ``exportResults(filename)`` (optional placeholder)

-----------------
LPOptimizer
-----------------

The first working solver. It formulates and solves a **linear program** to minimize total volume:

::

   minimize ∑ |Fᵢ| · Lᵢ

subject to:

- Equilibrium at each node
- Stress bounds (implicitly via bar usage)
- Non-negative cross-sectional areas

It returns an ``OptimizationResults`` object containing forces, lengths, volume, and active bars.

-----------------
OptimizationResults
-----------------

A container class that stores the full result of an optimization run:

- A vector of per-bar force results (``BarForceEntry``)
- Total volume and objective value
- Methods to:
  - Add entries
  - Compute total volume
  - Flag inactive bars

######################
Matrix Builder
######################

``MatrixBuilder`` builds the global equilibrium matrix ``A`` and load vector ``b`` such that:

::

   A · f = b

Each row enforces equilibrium at a node (in x or y), and each column corresponds to a bar. This forms the backbone of the linear optimization problem.

######################
Input & Output
######################

-----------------
InputParser
-----------------

Reads a ``.json`` input file containing:

- A list of nodes (with position, support flags, loads)
- A list of manually defined bars

This is useful for structured testing or comparing with analytical layouts. It’s also extendable to support automatic grid/arch generation in the future.

-----------------
OutputExporter
-----------------

Handles export of:

- SVG visualization of the truss
- CSV and JSON of optimization results

This helps with debugging, visualization, and integration with post-processing tools like Python.

######################
Main Files
######################

-----------------
main.cpp
-----------------

The entry point for production runs. It:

- Loads a structure from JSON using ``InputParser``
- Solves it using ``LPOptimizer``
- Exports results using ``OutputExporter``

-----------------
main_test.cpp
-----------------

Used for experiments and development. It:

- More explicitly defines the geometry of the structure to help with testing.

######################
Future Work
######################

- Actually implement the code
- Perhaps find a sensible place to implement templated functions
- Add support for full JSON-driven arch and GSM configuration (e.g. arch span/rise, grid resolution)
- Implement alternative solvers (e.g., ``MILPOptimizer`` for discrete bar selection)
- Integrate plotting libraries or output for Python-based visualization
- Support multiple load cases or parametric studies
- Add constraint filtering (e.g. bar angle restrictions, symmetry)
