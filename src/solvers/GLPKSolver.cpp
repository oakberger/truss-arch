#include "GLPKSolver.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>

GLPKSolver::GLPKSolver()
    : lp(glp_create_prob()), current_var_index(1), current_row_index(1)
{
    glp_set_obj_dir(lp, GLP_MIN);
    ia.push_back(0); ja.push_back(0); ar.push_back(0);  // 1-based indexing
}

GLPKSolver::~GLPKSolver() {
    glp_delete_prob(lp);
}

void GLPKSolver::initialize(int num_bars_,
                             int /*num_eqs*/,
                             const std::vector<int>& /*free_dofs*/,
                             const TrussStructure<2>& structure)
{
    this->num_bars = num_bars_;
    this->structure_ref = &structure;

    // Reserve space for variables
    glp_add_cols(lp, 2 * num_bars); // F_pos and F_neg

    for (int j = 0; j < 2 * num_bars; ++j) {
        glp_set_col_bnds(lp, j + 1, GLP_LO, 0.0, 0.0);

        double L = structure.bars[j % num_bars].length;
        double coef = (j < num_bars)
            ? L / structure.material.sigma_tension
            : L / structure.material.sigma_compression;

        glp_set_obj_coef(lp, j + 1, coef);
    }
}

void GLPKSolver::addVariable(const std::string& name, double lb, double ub) {
    glp_add_cols(lp, 1);
    glp_set_col_name(lp, current_var_index, name.c_str());
    glp_set_col_bnds(lp, current_var_index, GLP_DB, lb, ub);
    variable_map[name] = current_var_index;
    ++current_var_index;
}

void GLPKSolver::addConstraint(const std::vector<int>& indices,
                                const std::vector<double>& coeffs,
                                char sense, double rhs)
{
    int n = static_cast<int>(indices.size());
    std::vector<int> row_idx(n + 1);
    std::vector<double> row_val(n + 1);

    for (int i = 0; i < n; ++i) {
        row_idx[i + 1] = indices[i] + 1;
        row_val[i + 1] = coeffs[i];
    }

    glp_add_rows(lp, 1);
    if (sense == '=') {
        glp_set_row_bnds(lp, current_row_index, GLP_FX, rhs, rhs);
    } else if (sense == '<') {
        glp_set_row_bnds(lp, current_row_index, GLP_UP, -1e100, rhs);
    } else if (sense == '>') {
        glp_set_row_bnds(lp, current_row_index, GLP_LO, rhs, 1e100);
    } else {
        throw std::runtime_error("Invalid constraint sense");
    }

    glp_set_mat_row(lp, current_row_index, n, row_idx.data(), row_val.data());
    ++current_row_index;
}

void GLPKSolver::setObjective(const std::vector<double>& coeffs, bool minimize) {
    glp_set_obj_dir(lp, minimize ? GLP_MIN : GLP_MAX);
    for (int i = 0; i < static_cast<int>(coeffs.size()); ++i) {
        glp_set_obj_coef(lp, i + 1, coeffs[i]);
    }
}

void GLPKSolver::solve() {
    glp_simplex(lp, nullptr);

    solution.resize(num_bars);
    for (int j = 0; j < num_bars; ++j) {
        double Fp = glp_get_col_prim(lp, j + 1);
        double Fn = glp_get_col_prim(lp, j + 1 + num_bars);
        solution[j] = Fp - Fn;
    }
}

int GLPKSolver::getStatus() const {
    return glp_get_status(lp);
}

std::vector<double> GLPKSolver::getPrimalSolution() const {
    return solution;
}
