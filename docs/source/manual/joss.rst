---
title: Truss Arch Optimizer: A Ground Structure Method Implementation for Arch Truss Designs
tags:
  - C++
  - structural optimization
  - topology optimization
  - ground structure method
  - truss design
  - linear programming

authors:
  - name: Maximilian Eichenberger
    affiliation: 1
affiliations:
  - index: 1
    name: ETH Zürich, Institute for Building Materials
date: 30 May 2025
---

JOSS Article
============

Summary
-------

**Truss Arch Optimizer** is a C++ project developed as part of the ETH Zürich course "Towards Efficient and High-Performance Computing" for Engineers. It implements a classic form of topology optimization based on the Ground Structure Method (Bendsøe and Sigmund, 2003), targeting the automated layout optimization of truss structures within arch-like geometries.

The optimizer solves a linear programming (LP) problem derived from the plastic lower-bound formulation (Gallagher and Zienkiewicz, 1973; Gilbert and Tyas, 2003; Zhao, 2014), where the objective is to minimize total structural volume as a function of bar lengths, internal forces, and stress limits. Equilibrium constraints are enforced at all free degrees of freedom, but no stiffness or displacement constraints are included.

The software is modular and extensible, supporting customizable geometries, solver backends, and output formats. It provided a hands-on opportunity to apply high-performance computing principles and modern C++ design in the context of structural optimization.

Statement of Need
-----------------

Optimizing the layout of trusses is a key challenge in structural engineering, as substantial weight savings can be achieved by adjusting the structural configuration. The Ground Structure Method (GSM) provides a straightforward approach: it begins with a densely connected grid of nodes within a predefined design space and selects the most efficient subset of bars that satisfies static equilibrium.

Truss Arch Optimizer is a C++ implementation of this method, tailored to arch-like geometries. It applies the plastic lower-bound approach, formulating the problem as a linear program that balances internal axial forces against allowable material stress. It is designed as an exploratory tool that allows users to define the arch shape, generate the ground structure, specify boundary conditions, and compute material-efficient truss layouts.

Unlike more advanced frameworks, the current version of the optimizer does not enforce stiffness, displacement, or buckling constraints. As a result, most computed layouts are mechanisms: valid in terms of static equilibrium but physically unstable. Nevertheless, the tool offers insight into force paths and material distribution, and due to the extensible and modular layout, serves as a basis for adding further constraints.

Evaluation
----------

I think the strongest part of the project is its modular structure. Thanks to the way the course was set up — with time to plan the architecture before implementation, and clear specifications for documentation — I ended up with a fairly clean separation between components like the solver interface, optimizer logic, and export functions. Using Doxygen and Sphinx also helped reinforce good habits around code clarity and documentation. 

However as I am not very experienced with coding in C++ it is hard for me to assess if this project could perhaps have been implemented in a much more simple and concise way. I found a 99 line topology optimization MatLab code (Sigmund, 2001) that made me question the complexity of my code.

If I were to start over, I’d rethink how constraints are added to the system. Starting from a stiffness- or displacement-based formulation might have led to more physically meaningful results.

Outlook
-------

There are several directions in which this project could be extended. The most immediate next step would be to introduce additional constraints — starting with stiffness constraints to prevent mechanism-like solutions. This would likely require reworking parts of the solver logic and testing. From there, more advanced physical constraints such as buckling resistance could be considered.

One area I find particularly interesting is constructability. Optimizing structures to be not only efficient but also practical to build could involve strategies such as promoting repetition or assigning costs to nodes to discourage overly complex connections. For instance, Gilbert and Tyas (2003) discuss incorporating nodal costs as a way to enhance manufacturability, while Liu et al. (2023) explore the use of heuristic techniques and mixed-integer linear programming to enforce repetition through modular constraints.

Beyond constraints, the codebase could be extended to support 3D trusses. While parts of the system are already templated in dimension, logic related to ground structure generation and visualization would need to be generalized and likely redesigned.

Of course the code could also be extended to support a plethora of different arch-shapes and geometries. One idea would be to compute catenary curves under given loads and to use them to define the arch shape before running the truss optimization.

Finally, adding a Python interface (for either input configuration or post-processing) could make the tool significantly more accessible to non-C++ users.

References
----------

- Bendsøe, M. P., & Sigmund, O. (2003). *Topology Optimization: Theory, Methods, and Applications*. Springer. https://doi.org/10.1007/978-3-662-05086-6

- Gallagher, R. H., & Zienkiewicz, O. C. (Eds.). (1973). *Optimum Structural Design*. John Wiley & Sons.

- Zhao, X. (2014). *Topology Optimization of Truss Structures Using the Ground Structure Method* (Master's thesis, University of Illinois at Urbana-Champaign).

- Gilbert, M., & Tyas, A. (2003). Layout optimization of large-scale pin-jointed frames. *Engineering Computations*, 20(8), 1044–1064. https://doi.org/10.1108/02644400310503017

- Liu, Y., Wang, Z., Lu, H., Ye, J., Zhao, Y., & Xie, Y. M. (2023). Layout optimization of truss structures with modular constraints. *Structures*, 55, 1460–1469. https://doi.org/10.1016/j.istruc.2023.06.071

- Sigmund, O. (2001). A 99 line topology optimization code written in Matlab. *Structural and Multidisciplinary Optimization*, 21(2), 120–127. https://doi.org/10.1007/s001580050176
