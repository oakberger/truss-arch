#include "LPOptimizer.hpp"
#include "../utils/MatrixBuilder.hpp"
#include <glpk.h>
#include <cmath>
#include <iostream>

OptimizationResults LPOptimizer::solve(const TrussStructure& structure) {
    std::cout << "[LPOptimizer] Solving optimization problem using GLPK...\n";

    // 1. Build equilibrium matrix A and load vector b
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    MatrixBuilder::buildEquilibriumMatrix(structure, A, b);

    int num_eq = A.rows();      // 2 * num_nodes
    int num_bars = A.cols();    // m
    int num_vars = 2 * num_bars; // F_pos and F_neg for each bar

    // 2. Create LP problem
    glp_prob* lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MIN); // objective of glpk problem is to minimize

    // 3. Add equilibrium constraints: A * F = b
    glp_add_rows(lp, num_eq);
    for (int i = 0; i < num_eq; ++i) {
        glp_set_row_bnds(lp, i + 1, GLP_FX, b[i], b[i]);
    }

    // 4. Add variables: F_pos and F_neg for each bar (all >= 0)
    glp_add_cols(lp, num_vars);
    for (int j = 0; j < num_vars; ++j) {
        glp_set_col_bnds(lp, j + 1, GLP_LO, 0.0, 0.0); // ≥ 0
        glp_set_obj_coef(lp, j + 1, structure.bars[j % num_bars].length);
    }

    // 5. Setup the constraint matrix
    // GLPK uses 1-based indexing for ia/ja/ar.
    std::vector<int> ia(1); // 0-th entry unused
    std::vector<int> ja(1);
    std::vector<double> ar(1);
    for (int i = 0; i < num_eq; ++i) {
        for (int j = 0; j < num_bars; ++j) {
            double val = A(i, j);
            if (std::abs(val) < 1e-12) continue;
            // F = F_pos - F_neg → A*(F_pos - F_neg) = b
            ia.push_back(i + 1); ja.push_back(j + 1); ar.push_back(val);
            ia.push_back(i + 1); ja.push_back(j + 1 + num_bars); ar.push_back(-val);
        }
    }

    glp_load_matrix(lp, ia.size() - 1, ia.data(), ja.data(), ar.data());

    // 6. Solve LP
    glp_simplex(lp, nullptr);

    // 7. Extract solution
    OptimizationResults results;
    for (int j = 0; j < num_bars; ++j) {
        double Fp = glp_get_col_prim(lp, j + 1);
        double Fn = glp_get_col_prim(lp, j + 1 + num_bars);
        double force = Fp - Fn;
        double area = std::abs(force) / 1.0;  // σ_max = 1.0
        results.addEntry(structure.bars[j].id, force, structure.bars[j].length);
    }

    glp_delete_prob(lp);

    results.computeTotalVolume();
    results.markActiveBars();
    return results;
}

