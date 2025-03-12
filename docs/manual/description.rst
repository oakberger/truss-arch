.. image:: /docs/images/Truss-Arch-Bridge-Porto.jpeg
   :alt: Truss Arch Bridge
   :width: 500px
   :align: center

######################
Project Objectives
######################

The aim of this project is to develop a **C++ program** for optimizing the layout of a **truss arch** using the **ground structure method**. The goal is to **minimize material usage** while ensuring the structure remains **stable under given loads**.

Initially, I will focus on implementing a **working truss optimization code**, ensuring it efficiently determines an **optimal layout**. Once that is in place, if time permits, I thought it would be interesting to introduce **modular constraints** to improve **constructability**. (Probably won't have enough time but still thought it was interesting).

Note: I find it difficult to asses how much time / how difficult it will be to implement this. This is why I am unsure what the exact scope of the project should be.

######################
Development Plan
######################

--------------------------------------
1. Implement the Ground Structure Method (Core Optimization)
--------------------------------------

- Generate a **graph-based representation** of potential truss connections.
- Use **Mixed-Integer Linear Programming** or a **heuristic search algorithm** to determine the optimal subset of connections that minimize material while meeting structural constraints.

--------------------------------------
2. Validate the Structure Using Finite Element Analysis (FEA)
--------------------------------------

- Compute **forces, stresses, and displacements** to ensure the optimized truss meets stability and strength requirements.
- Utilize libraries like **Eigen** or integrate an existing **FEA solver** for accuracy.

--------------------------------------
3. Enhance Constructability with Modular Constraints (If Time Permits)
--------------------------------------

- Introduce **modular design principles** to group similar elements, reducing manufacturing complexity and cost.
- Apply **clustering algorithms** or **integer programming techniques** to enforce standardization.

######################
Theoretical Foundation
######################

The optimization process relies on **structural layout optimization principles**, specifically using the **ground structure method**, which generates an initial set of potential truss members. The optimization process involves selecting an **optimal subset of these members** through **Mixed-Integer Linear Programming (MILP)** or **heuristic search techniques** to achieve the most efficient structural configuration.