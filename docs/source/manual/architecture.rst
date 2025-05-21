Architecture Overview
=====================

Truss Arch Optimizer is a modular C++ project that implements a simplified form of topology optimization using the Ground Structure Method (GSM) — specifically for arch-like structures. The goal is to generate a network of nodes and bars within a given geometry and determine the optimal truss layout by minimizing total material volume via linear programming.

Beyond its engineering purpose, this project also served as a hands-on opportunity to deepen my understanding of C++ programming and apply the principles from the course “Towards Efficient and High-Performance Computing for Engineers” at ETH Zürich.


Core Concept
------------

At its heart is the ``TrussStructure`` class, which holds everything from geometry to material properties. Around it are key modules that handle:

- Optimization (e.g., LP)
- Constraint setup (e.g., equilibrium)
- Geometry definitions (e.g., arch shapes)
- Input/output in JSON, SVG, and CSV


Class Overview
--------------

**Model**  
Manages all node-related data using vectors (`coords`, `support`, `load`). Each node is referenced by its index.

**Bar**  
Represents a possible connection between two nodes, storing start/end indices, length, internal force, area, and an active flag.

**TrussStructure**  
Owns the full structure. Key responsibilities:

- Add nodes and bars
- Generate a rectangular grid
- Build ground structures, optionally constrained within an arch


Arch Geometry
-------------

**BaseArchShape & ParallelArchShape**  
Defines the arch domain via `outerY(x)` and `innerY(x)`. Nodes and bars are only added if they fall within this band. This keeps the optimized truss realistic and visually pleasing.


Optimization Setup
------------------

**BaseOptimizer**  
Abstract interface for solvers. Just one function to implement: `solve()`.

**LPOptimizer**  
Solves a linear program to minimize:

::

   total_volume = ∑ |Fᵢ| · Lᵢ / σ

Constraints include:

- Equilibrium at each node
- Implicit stress limits via material properties
- Non-negativity of area (enforced by design)

**Constraint**  
Base class for any constraint. Each derived constraint (e.g., `EquilibriumConstraint`) adds its rows to the solver via the builder. 
This allows for easy addition of new constraints in the future.

**OptimizationResults**  
Stores results from an optimization run:

- Bar forces and areas
- Total volume
- Methods to compute volume and flag active bars


Matrix Builder
--------------

**MatrixBuilder**  
Constructs the equilibrium equations (``A · f = b``) from the current structure and passes them to the solver interface.


Input & Output
--------------

**InputParser**  
Reads a JSON file defining either a rectangle or an arch-based structure. Inputs include geometry, material properties, boundary conditions, and output paths.

**OutputExporter**  
Exports SVG visualizations and CSV/JSON data for post-processing or inspection.


Main Files (examples)
---------------------

Different examples are provided in the `examples` folder. Each example is a C++ file that demonstrates the usage of the library. The examples include:



Test Files
----------

Tests are located in the `tests` folder. The idea of the tests are to check the functionality of individual features. This is used to speed up the development and testing process.


Possible future Features
------------------------

- Add different solver types (e.g., MILP)
- More constraint types (e.g., symmetry, buckling, node costs (constructability))
- Add full support for 3D structures
- Improve visualization (Tension/Compression in different colors etc.)
- Python-based visualization output


