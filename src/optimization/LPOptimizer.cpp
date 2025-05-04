#include "LPOptimizer.hpp"
#include "../utils/MatrixBuilder.hpp"
#include <glpk.h>
#include <cmath>
#include <iostream>

OptimizationResults LPOptimizer::solve(const TrussStructure& structure) {
    std::cout << "[LPOptimizer] Solving optimization problem using GLPK...\n";

    // 1. Build full equilibrium matrix A and load vector b
    Eigen::MatrixXd A_full;
    Eigen::VectorXd b_full;
    MatrixBuilder::buildEquilibriumMatrix(structure, A_full, b_full);

    const auto& model = structure.model;
    const size_t num_nodes = model.nodeCount();
    const size_t num_bars = structure.bars.size();
    const size_t total_dofs = 2 * num_nodes;

    // 2. Extract only free DOFs
    std::vector<int> free_dofs;
    for (size_t i = 0; i < num_nodes; ++i) {
        if (!model.support_x[i]) free_dofs.push_back(2 * i);
        if (!model.support_y[i]) free_dofs.push_back(2 * i + 1);
    }
    std::cout << "[DEBUG] Free DOFs: " << free_dofs.size() << "\n";

    const int num_eq = free_dofs.size();
    const int num_vars = 2 * num_bars; // F_pos and F_neg for each bar

    // Create reduced A and b
    Eigen::MatrixXd A(num_eq, num_bars);
    Eigen::VectorXd b(num_eq);
    for (size_t i = 0; i < free_dofs.size(); ++i) {
        A.row(i) = A_full.row(free_dofs[i]);
        b(i) = b_full(free_dofs[i]);
    }

    std::cout << "Max A(i,j): " << A.maxCoeff() << ", min: " << A.minCoeff() << "\n";
    std::cout << "Num equations: " << num_eq << ", num bars: " << num_bars << "\n";

    // 3. Create LP problem
    glp_prob* lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MIN);

    glp_add_rows(lp, num_eq);
    for (int i = 0; i < num_eq; ++i) {
        glp_set_row_bnds(lp, i + 1, GLP_FX, b[i], b[i]);
    }

    glp_add_cols(lp, num_vars);
    for (int j = 0; j < num_vars; ++j) {
        glp_set_col_bnds(lp, j + 1, GLP_LO, 0.0, 0.0);
        const double L = structure.bars[j % num_bars].length;
        const double coef = (j < num_bars)
            ? L / structure.material.sigma_tension   // s⁺
            : L / structure.material.sigma_compression; // s⁻

        glp_set_obj_coef(lp, j + 1, coef);
    }

    // 4. Populate constraint matrix
    std::vector<int> ia(1);
    std::vector<int> ja(1);
    std::vector<double> ar(1);

    for (int i = 0; i < num_eq; ++i) {
        for (int j = 0; j < num_bars; ++j) {
            double val = A(i, j);
            if (std::abs(val) < 1e-12) continue;
            ia.push_back(i + 1); ja.push_back(j + 1); ar.push_back(val);
            ia.push_back(i + 1); ja.push_back(j + 1 + num_bars); ar.push_back(-val);
        }
    }

    glp_load_matrix(lp, ia.size() - 1, ia.data(), ja.data(), ar.data());

    glp_write_lp(lp, nullptr, "debug.lp");

    for (const auto& bar : structure.bars) {
        std::cout << "Bar " << bar.id << " length: " << bar.length << "\n";
    }

    std::cout << "Nonzeros in A matrix: " << (ia.size() - 1) << "\n";
    std::cout << "Expected rows: " << num_eq << ", columns: " << num_vars << "\n";

    // 5. Solve
    glp_simplex(lp, nullptr);
    int status = glp_get_status(lp);
    std::cout << "GLPK status: " << status << "\n";

    // 6. Extract solution
    OptimizationResults results;
    for (int j = 0; j < num_bars; ++j) {
        double Fp = glp_get_col_prim(lp, j + 1);
        double Fn = glp_get_col_prim(lp, j + 1 + num_bars);
        double force = Fp - Fn;
        const double sigma = (force >= 0)
            ? structure.material.sigma_tension
            : structure.material.sigma_compression;
        double area = std::abs(force) / sigma;
        results.addEntry(structure.bars[j].id, force, structure.bars[j].length);
    }

    glp_delete_prob(lp);

    results.computeTotalVolume();
    results.markActiveBars();
    return results;
}
