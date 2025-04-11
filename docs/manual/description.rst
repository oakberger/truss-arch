.. image:: /docs/images/Truss-Arch-Bridge-Porto.jpeg
   :alt: Truss Arch Bridge
   :width: 500px
   :align: center

######################
Project Objectives
######################

The aim of this project is to develop a **C++ program** that optimizes the layout of a **truss arch** using the **Ground Structure Method (GSM)**. The goal is to **minimize material usage** while ensuring that the structure remains **statically stable under given loads**.

The program constructs a truss within a defined **arch region**, represented as a parabolic band with configurable span, rise, and thickness. Using a **grid of potential node locations**, it generates all feasible bars within a maximum length threshold, and solves a **linear programming problem** to find the optimal subset of bars to carry the load.

The first priority is to implement a working, flexible truss optimization system. If time allows, further constraints could be added (e.g. limiting bar angles, enforcing modularity, or restricting bar types), but these are not currently in scope.

*Note: It's hard to estimate the total effort involved since this includes both geometry generation and optimization modeling — so the scope may evolve slightly depending on progress.*

######################
Development Plan
######################

--------------------------------------
1. Implement the Ground Structure Method (Core Optimization)
--------------------------------------

- Define an **arch-shaped geometry** and generate a **grid of nodes** inside it.
- Connect nodes using distance-based criteria to form the **ground structure**.
- Solve a **Linear Programming (LP)** problem to minimize total material (volume), subject to:
  - Equilibrium constraints at all nodes
  - Non-negativity constraints on cross-sectional areas
- Return and export **forces**, **bar lengths**, and **used members**.

--------------------------------------
2. Visualization and Result Output
--------------------------------------

- Output results to **SVG**, **CSV**, and **JSON** using a modular `OutputExporter`.
- Use SVG to visualize the geometry and activated bars, and CSV/JSON for deeper analysis or post-processing in Python.

--------------------------------------
3. Add Geometry Constraints (Arch Region)
--------------------------------------

- Ensure that both **nodes and bars** stay **within a parabolic arch region** using the `ArchShape` class.
- Provide easy configuration of **arch span**, **rise**, **thickness**, and **grid resolution** through JSON or code-based setup.

--------------------------------------
4. Optional: Extend with Modular Constraints (If Time Permits)
--------------------------------------

- Introduce optional constraints to improve **constructability**, such as grouping bar lengths or angles.
- This may involve **clustering**, **rounded length bins**, or additional constraints in the LP/MILP formulation.

######################
Theoretical Foundation
######################

This project is based on **structural layout optimization**, with a focus on the **Ground Structure Method** (GSM). GSM starts with a dense network of potential truss members (bars), and uses mathematical programming (here: **Linear Programming**) to identify the most efficient layout under a given set of loads and supports.

The optimization objective is to minimize the **total material volume**, modeled as:

$ \min \sum |F_i| \cdot L_i $

subject to:
- Static equilibrium at each node
- Bar forces within valid limits
- Only non-negative cross-sectional areas

The result is a **material-efficient** and **statically sound** truss layout within the geometric constraints of an arch.

