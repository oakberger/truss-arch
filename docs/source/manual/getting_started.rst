#######################
Getting Started
#######################

This guide should help you build and run the **Truss Arch Optimizer**.

=======================
Prerequisites
=======================

Make sure the following are installed:

- CMake ≥ 3.16
- A C++17 compatible compiler (e.g., ``g++``, ``clang++``)
- Eigen3 (header-only)
- GLPK (library and headers)
- nlohmann/json (installed with CMake support)
- Python ≥ 3.8 (only required for building Sphinx documentation)
- Sphinx (optional, for documentation)

=======================
Build Instructions
=======================

1. Clone or extract the project 

2. Configure and build the project (Cmake)

3. Run the examples

The examples are located in the ``examples`` folder. They can be run with:

.. code-block:: bash

    ./build/example_name

The input for the examples are located in ``examples/input``. 

The output is a .svg, .csv and .json file and will be saved in the ``examples/output`` folder. The .svg file shows the final truss layout, where the thickness of each bar either represents the amount of force in each bar or the scaled area of the bar.

=======================
Documentation
=======================

Make sure in ``ctrl+shift+p --> Cmake: Edit Cmake Cache (UI)``, TRUSS_DOC and TRUSS_SPHINX_DOC are set to ON.

Building with Cmake should then automaticallyy generate the Doxygen and Sphinx documentation. In case it doesn't, make sure you have the correct python and sphinx installed (brew install sphinx worked for me). It can manualy be built with:

.. code-block:: bash

    cmake --build build --target doc_doxygen

    cmake --build build --target sphinx_docs

The documentation can be viewed by opening the html files ``build/docs/doxygen/html/index.html`` and ``build/docs/sphinx/index.html``.