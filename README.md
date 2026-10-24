# Truss Arch Optimizer

**Layout optimization of truss arches using the Ground Structure Method, written in modern C++.**

<p align="center">
  <img src="images/Truss-Arch-Bridge-Porto.jpeg" alt="Truss arch bridge" width="500">
</p>

Truss Arch Optimizer generates a dense network of candidate bars inside an arch-shaped design domain and solves a linear program to find the layout that carries the given loads with the **minimum material volume**. It was developed as part of the ETH Zürich course *Towards Efficient and High-Performance Computing for Engineers*, with a focus on clean, modular C++ design.

---

## Features

- **Ground Structure Method (GSM)**: builds a grid of candidate nodes and connects them with all feasible bars up to a maximum length
- **Arch-shaped design domains**: nodes and bars are kept within a parabolic band with configurable span, rise and thickness
- **Linear programming** formulation based on the plastic lower-bound theorem
- **Solver-agnostic**: works with **GLPK** out of the box, with optional support for **Gurobi**
- **JSON input** for geometry, material properties, boundary conditions and output paths
- **SVG, CSV and JSON output** for visualization and post-processing
- **Extensible constraint system**: new constraints can be added by subclassing a single base class
- **Documentation** generated with Doxygen and Sphinx

---

## How it works

1. **Define the domain:** an arch region is described by an outer and inner curve, `outerY(x)` and `innerY(x)`.
2. **Generate the ground structure:** a grid of nodes is placed inside the arch, and every pair of nodes within a maximum distance is connected by a candidate bar, provided the bar stays inside the domain.
3. **Optimize:** a linear program selects the bar forces that minimize total volume while satisfying equilibrium.
4. **Export:** active bars, forces, areas and the total volume are written to SVG, CSV and JSON.

### Formulation

The objective is to minimize the total material volume:

$$
V = \sum_i \frac{|F_i| \, L_i}{\sigma}
$$

subject to static equilibrium at every free degree of freedom:

$$
A \, f = b
$$

where

- $A$ is the **equilibrium matrix**, whose entries are the direction cosines of the bars connected to each free DOF,
- $f$ is the vector of **bar forces**, split into tensile ($f^+$) and compressive ($f^-$) parts so the problem stays linear,
- $b$ is the vector of **external loads** at the free DOFs.

Cross-sectional areas follow from the forces and the allowable stress $\sigma$, and are non-negative by construction.

> **Note:** The current formulation enforces equilibrium only, without stiffness, displacement or buckling constraints. The resulting layouts show efficient force paths, but many of them are **mechanisms**, i.e. statically valid but unstable without additional bracing.

---

## Getting started

### Prerequisites

| Dependency | Notes |
|---|---|
| CMake ≥ 3.16 | |
| C++17 compiler | e.g. `g++` or `clang++` |
| Eigen3 | header-only |
| GLPK | library and headers |
| nlohmann/json | installed with CMake support |
| Gurobi | *optional*, faster solver |
| Python ≥ 3.8 + Sphinx | *optional*, only for building the documentation |

### Build

```bash
git clone https://github.com/<your-username>/truss-arch.git
cd truss-arch
cmake -S . -B build
cmake --build build
```

### Run an example

Example programs live in the `examples/` folder and are built alongside the library:

```bash
./build/<example_name>
```

- **Input:** JSON files in `examples/input/`
- **Output:** `.svg`, `.csv` and `.json` files in `examples/output/`

In the SVG output, the thickness of each bar represents either the force in the bar or its scaled cross-sectional area.

---

## Architecture

The project is built around a central `TrussStructure` class, with separate modules for geometry, optimization, constraints and I/O.

| Component | Responsibility |
|---|---|
| `Model` | Stores node data (`coords`, `support`, `load`), indexed by node |
| `Bar` | A candidate connection between two nodes: length, force, area, active flag |
| `TrussStructure` | Owns the structure; adds nodes and bars, generates grids and ground structures |
| `BaseArchShape`, `ParallelArchShape` | Define the arch domain via `outerY(x)` and `innerY(x)` |
| `BaseOptimizer`, `LPOptimizer` | Optimizer interface (`solve()`) and the LP implementation |
| `Constraint`, `EquilibriumConstraint` | Pluggable constraints that add their rows to the problem |
| `MatrixBuilder` | Assembles the equilibrium system $A f = b$ |
| `SolverInterface` | Abstract LP solver API, implemented by `GLPKSolver` and `GurobiSolver` |
| `OptimizationResults` | Bar forces, areas, total volume and active-bar flags |
| `InputParser` | Reads rectangular or arch-based problem definitions from JSON |
| `OutputExporter` | Writes SVG visualizations and CSV/JSON data |

Thanks to the abstract `SolverInterface`, a new solver backend can be added by subclassing it and implementing the required methods, without touching the rest of the code.

---

## Tests

Unit tests for individual features are located in the `tests/` folder.

---

## Documentation

To generate the Doxygen and Sphinx documentation, set `TRUSS_DOC` and `TRUSS_SPHINX_DOC` to `ON` in the CMake cache. In VS Code: `Ctrl+Shift+P` → **CMake: Edit CMake Cache (UI)**.

Building the project then generates the documentation automatically. To build it manually:

```bash
cmake --build build --target doc_doxygen
cmake --build build --target sphinx_docs
```

Then open:

- `build/docs/doxygen/html/index.html`
- `build/docs/sphinx/index.html`

If the Sphinx build fails, check that Python and Sphinx are installed (on macOS: `brew install sphinx-doc`).

---

## Roadmap

- **Stiffness and displacement constraints** to avoid mechanism-like solutions
- **Buckling** constraints for compression members
- **Constructability**: nodal costs and modular/repetition constraints (e.g. via MILP)
- **More arch geometries**, e.g. catenary shapes computed from the applied loads
- **3D trusses**: generalizing ground structure generation and visualization
- **Improved visualization**, e.g. tension and compression in different colors
- **Python interface** for input configuration and post-processing

---

## References

- Bendsøe, M. P., & Sigmund, O. (2003). *Topology Optimization: Theory, Methods, and Applications*. Springer. [doi:10.1007/978-3-662-05086-6](https://doi.org/10.1007/978-3-662-05086-6)
- Gallagher, R. H., & Zienkiewicz, O. C. (Eds.). (1973). *Optimum Structural Design*. John Wiley & Sons.
- Gilbert, M., & Tyas, A. (2003). Layout optimization of large-scale pin-jointed frames. *Engineering Computations*, 20(8), 1044–1064. [doi:10.1108/02644400310503017](https://doi.org/10.1108/02644400310503017)
- Liu, Y., Wang, Z., Lu, H., Ye, J., Zhao, Y., & Xie, Y. M. (2023). Layout optimization of truss structures with modular constraints. *Structures*, 55, 1460–1469. [doi:10.1016/j.istruc.2023.06.071](https://doi.org/10.1016/j.istruc.2023.06.071)
- Sigmund, O. (2001). A 99 line topology optimization code written in Matlab. *Structural and Multidisciplinary Optimization*, 21(2), 120–127. [doi:10.1007/s001580050176](https://doi.org/10.1007/s001580050176)
- Zhao, X. (2014). *Topology Optimization of Truss Structures Using the Ground Structure Method*. Master's thesis, University of Illinois at Urbana-Champaign. [hdl:2142/50452](https://hdl.handle.net/2142/50452)

---

## Author

**Maximilian Eichenberger**, ETH Zürich, Institute for Building Materials

Developed for the course *Towards Efficient and High-Performance Computing for Engineers* at ETH Zürich (2025).
