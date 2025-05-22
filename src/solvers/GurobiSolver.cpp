#include "GurobiSolver.hpp"
#include <iostream>

GurobiSolver::GurobiSolver()
    : env(), model(env) {
    model.set(GRB_IntParam_OutputFlag, 0); // Suppress console output
}

GurobiSolver::~GurobiSolver() {}

void GurobiSolver::initialize(int num_bars_, int /*num_eqs*/,
                              const std::vector<int>& /*free_dofs*/,
                              const TrussStructure<2>& structure) {
    this->num_bars = num_bars_;
    this->structure_ref = &structure;

    force_pos_vars.resize(num_bars);
    force_neg_vars.resize(num_bars);

    for (int j = 0; j < num_bars; ++j) {
        double L = structure.bars[j].length;
        double obj_pos = L / structure.material.sigma_tension;
        double obj_neg = L / structure.material.sigma_compression;

        force_pos_vars[j] = model.addVar(0.0, GRB_INFINITY, obj_pos, GRB_CONTINUOUS);
        force_neg_vars[j] = model.addVar(0.0, GRB_INFINITY, obj_neg, GRB_CONTINUOUS);
    }

    model.update(); // Integrate new variables
}

void GurobiSolver::addConstraint(const std::vector<int>& indices,
                                 const std::vector<double>& coeffs,
                                 char sense, double rhs) {
    GRBLinExpr expr = 0.0;

    for (size_t k = 0; k < indices.size(); ++k) {
        int idx = indices[k];
        double coeff = coeffs[k];

        if (idx < num_bars) {
            expr += coeff * force_pos_vars[idx];
        } else {
            expr += coeff * force_neg_vars[idx - num_bars];
        }
    }

    if (sense == '=') {
        model.addConstr(expr == rhs);
    } else if (sense == '<') {
        model.addConstr(expr <= rhs);
    } else if (sense == '>') {
        model.addConstr(expr >= rhs);
    } else {
        throw std::runtime_error("Invalid constraint sense passed to GurobiSolver");
    }
}

void GurobiSolver::solve() {
    try {
        model.optimize();
        gurobi_status = model.get(GRB_IntAttr_Status);
        if (gurobi_status != GRB_OPTIMAL) {
            std::cerr << "[GurobiSolver] Warning: Solver finished with non-optimal status code: "
                      << gurobi_status << "\n";
        }
    } catch (GRBException& e) {
        std::cerr << "[GurobiSolver] Gurobi exception: " << e.getMessage() << "\n";
        gurobi_status = -1;
    } catch (...) {
        std::cerr << "[GurobiSolver] Unknown error during optimization.\n";
        gurobi_status = -1;
    }
}

std::vector<double> GurobiSolver::getPrimalSolution() const {
    std::vector<double> result(num_bars);

    for (int j = 0; j < num_bars; ++j) {
        double pos = force_pos_vars[j].get(GRB_DoubleAttr_X);
        double neg = force_neg_vars[j].get(GRB_DoubleAttr_X);
        result[j] = pos - neg;
    }

    return result;
}

int GurobiSolver::getStatus() const {
    return gurobi_status;
}

void GurobiSolver::addVariable(const std::string&, double, double) {
    throw std::runtime_error("GurobiSolver::addVariable is not used.");
}

void GurobiSolver::setObjective(const std::vector<double>&, bool) {
    throw std::runtime_error("GurobiSolver::setObjective is not used.");
}
