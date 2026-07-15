#include "../include/cqq/operations.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <execution>
#include <random>
#include <ranges>
#include <stdexcept>

namespace cqq {

void apply_hadamard(QStateVector& qstate, unsigned target) {
    const auto mask = bit_mask(target);
    const auto inv_sqrt2 = 1.0 / std::sqrt(2.0);

    for (size_t i = 0; i < qstate.size(); ++i) {
        if ((i & mask) == 0) {
            size_t j = i | mask;

            std::complex<double> temp = qstate[i];
            qstate[i] = (temp + qstate[j]) * inv_sqrt2;
            qstate[j] = (temp - qstate[j]) * inv_sqrt2;
        }
    }
}

void apply_pauli_x(QStateVector& qstate, unsigned target) {
    const auto mask = bit_mask(target);

    for (size_t i = 0; i < qstate.size(); ++i) {
        if ((i & mask) == 0) {
            size_t j = i | mask;
            std::swap(qstate[i], qstate[j]);
        }
    }
}

void apply_pauli_y(QStateVector& qstate, unsigned target) {
    const auto mask = bit_mask(target);

    for (size_t i = 0; i < qstate.size(); ++i) {
        if ((i & mask) == 0) {
            size_t j = i | mask;

            std::complex<double> temp = qstate[i];
            qstate[i] = std::complex<double>(0, -1) * qstate[j];
            qstate[j] = std::complex<double>(0, 1) * temp;
        }
    }
}

void apply_pauli_z(QStateVector& qstate, unsigned target) {
    const auto mask = bit_mask(target);

    for (size_t i = 0; i < qstate.size(); ++i) {
        if ((i & mask)) {
            qstate[i] = -qstate[i];
        }
    }
}

void apply_controlled_not(QStateVector& qstate, unsigned control, unsigned target) {
    const auto cmask = bit_mask(control);
    const auto tmask = bit_mask(target);

    for (size_t i = 0; i < qstate.size(); ++i) {
        if ((i & cmask) && !(i & tmask)) {
            size_t j = i | tmask;
            std::swap(qstate[i], qstate[j]);
        }
    }
}

void apply_swap(QStateVector& qstate, unsigned q1, unsigned q2) {
    if (q1 == q2)
        return;

    const auto m1 = bit_mask(q1);
    const auto m2 = bit_mask(q2);

    for (size_t i = 0; i < qstate.size(); ++i) {
        if ((i & m1) != (i & m2)) {
            size_t j = i ^ (m1 | m2);

            if (i < j) {
                std::swap(qstate[i], qstate[j]);
            }
        }
    }
}

void measure(QStateVector& qstate, unsigned qubit, unsigned& creg) {
    if (qubit >= num_qubits(qstate)) {
        throw std::invalid_argument("Qubit index out of range.");
    }

    const auto mask = bit_mask(qubit);

    double prob_0 = 0.0;
    for (size_t i = 0; i < qstate.size(); ++i) {
        if ((i & mask) == 0) {
            prob_0 += std::norm(qstate[i]);
        }
    }

    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution dist(prob_0);
    unsigned result = dist(rng) ? 0 : 1;
    creg = result;

    double normal_factor = 0.0;
    for (size_t i = 0; i < qstate.size(); ++i) {
        if (((i & mask) ? 1 : 0) != result) {
            qstate[i] = std::complex<double>(0.0, 0.0);
        } else {
            normal_factor += std::norm(qstate[i]);
        }
    }

    double inv_sqrt_normal_factor = 1.0 / std::sqrt(normal_factor);
    for (size_t i = 0; i < qstate.size(); ++i) {
        qstate[i] *= inv_sqrt_normal_factor;
    }
}

} // namespace cqq
