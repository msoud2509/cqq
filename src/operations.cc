#include "../include/cqq/operations.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <execution>
#include <random>
#include <ranges>
#include <stdexcept>

namespace cqq {

template <typename Precision>
void apply_hadamard(QStateVector<Precision>& qstate, unsigned target) {
    const auto mask = bit_mask(target);
    const size_t step = mask << 1;
    const auto inv_sqrt2 = static_cast<Precision>((1.0) / std::sqrt(2.0));

    for (size_t block = 0; block < qstate.size(); block += step) {
        for (size_t i = 0; i < mask; ++i) {
            const size_t idx0 = block + i;
            const size_t idx1 = idx0 + mask;

            std::complex<Precision> v0 = qstate[idx0];
            std::complex<Precision> v1 = qstate[idx1];

            qstate[idx0] = (v0 + v1) * inv_sqrt2;
            qstate[idx1] = (v0 - v1) * inv_sqrt2;
        }
    }
}

template <typename Precision> void apply_pauli_x(QStateVector<Precision>& qstate, unsigned target) {
    const size_t mask = bit_mask(target);
    const size_t step = mask << 1;

    for (size_t block = 0; block < qstate.size(); block += step) {
        for (size_t i = 0; i < mask; ++i) {
            std::swap(qstate[block + i], qstate[block + i + mask]);
        }
    }
}

template <typename Precision> void apply_pauli_y(QStateVector<Precision>& qstate, unsigned target) {
    const auto mask = bit_mask(target);
    const size_t step = mask << 1;

    for (size_t block = 0; block < qstate.size(); block += step) {
        for (size_t i = 0; i < mask; ++i) {
            const size_t idx0 = block + i;
            const size_t idx1 = idx0 + mask;

            std::complex<Precision> temp = qstate[idx0];
            qstate[idx0] = std::complex<Precision>(0, -1) * qstate[idx1];
            qstate[idx1] = std::complex<Precision>(0, 1) * temp;
        }
    }
}

template <typename Precision> void apply_pauli_z(QStateVector<Precision>& qstate, unsigned target) {
    const auto mask = bit_mask(target);
    const size_t step = mask << 1;

    for (size_t block = 0; block < qstate.size(); block += step) {
        for (size_t i = 0; i < mask; ++i) {
            // add by mask to offset calculation to the second half of the block (where target is 1)
            qstate[block + i + mask] = -qstate[block + i + mask];
        }
    }
}

template <typename Precision>
void apply_controlled_not(QStateVector<Precision>& qstate, unsigned control, unsigned target) {
    const size_t low_bit  = std::min(control, target);
    const size_t high_bit = std::max(control, target);

    const auto low_mask  = bit_mask(low_bit);
    const auto high_mask = bit_mask(high_bit);

    const auto cmask = bit_mask(control);
    const auto tmask = bit_mask(target);

    const auto step_low  = low_mask << 1;
    const auto step_high = high_mask << 1;

    for (size_t high = 0; high < qstate.size(); high += step_high) {
        for (size_t mid = 0; mid < high_mask; mid += step_low) {
            for (size_t low = 0; low < low_mask; ++low) {
                const size_t base = high | mid | low;

                size_t i = base | cmask; 
                i &= ~tmask; 

                size_t j = i | tmask;

                std::swap(qstate[i], qstate[j]);
            }
        }
    }
}

template <typename Precision>
void apply_swap(QStateVector<Precision>& qstate, unsigned q1, unsigned q2) {
    if (q1 == q2)
        return;

    const size_t low_bit  = std::min(q1, q2);
    const size_t high_bit = std::max(q1, q2);

    const size_t low_mask  = bit_mask(low_bit);
    const size_t high_mask = bit_mask(high_bit);

    const size_t step_low  = low_mask << 1;
    const size_t step_high = high_mask << 1;

    for (size_t high = 0; high < qstate.size(); high += step_high) {
        for (size_t mid = 0; mid < high_mask; mid += step_low) {
            for (size_t low = 0; low < low_mask; ++low) {
                size_t base = high | mid | low;
                size_t i = base | low_mask;
                size_t j = base | high_mask;

                std::swap(qstate[i], qstate[j]);
            }
        }
    }
}

template <typename Precision>
void measure(QStateVector<Precision>& qstate, unsigned qubit, unsigned& creg) {
    if (qubit >= num_qubits(qstate)) {
        throw std::invalid_argument("Qubit index out of range.");
    }

    const auto mask = bit_mask(qubit);
    const auto step = mask << 1;

    Precision prob_0 = 0.0;
    for (size_t block = 0; block < qstate.size(); block += step) {
        for (size_t i = 0; i < mask; ++i) {
            prob_0 += std::norm(qstate[block + i]);
        }
    }

    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution dist(prob_0);
    unsigned result = dist(rng) ? 0 : 1;
    creg = result;

    // collapse remaining state and normalize
    Precision normal_factor = (result == 0) ? prob_0 : (static_cast<Precision>(1.0) - prob_0);
    Precision inv_sqrt_normal_factor = static_cast<Precision>(1.0) / std::sqrt(normal_factor);

    if (result == 0) {
        for (size_t block = 0; block < qstate.size(); block += step) {
            for (size_t i = 0; i < mask; ++i) {
                qstate[block + i] *= inv_sqrt_normal_factor;
                qstate[block + i + mask] = std::complex<Precision>(0.0, 0.0);
            }
        }
    }
    else {
        for (size_t block = 0; block < qstate.size(); block += step) {
            for (size_t i = 0; i < mask; ++i) {
                qstate[block + i] = std::complex<Precision>(0.0, 0.0);
                qstate[block + i + mask] *= inv_sqrt_normal_factor;
            }
        }
    }
}

// instantiation
template void apply_hadamard(QStateVector<double>& qstate, unsigned target);
template void apply_pauli_x(QStateVector<double>& qstate, unsigned target);
template void apply_pauli_y(QStateVector<double>& qstate, unsigned target);
template void apply_pauli_z(QStateVector<double>& qstate, unsigned target);
template void apply_controlled_not(QStateVector<double>& qstate, unsigned control, unsigned target);
template void apply_swap(QStateVector<double>& qstate, unsigned q1, unsigned q2);
template void measure(QStateVector<double>& qstate, unsigned qubit, unsigned& creg);

template void apply_hadamard(QStateVector<float>& qstate, unsigned target);
template void apply_pauli_x(QStateVector<float>& qstate, unsigned target);
template void apply_pauli_y(QStateVector<float>& qstate, unsigned target);
template void apply_pauli_z(QStateVector<float>& qstate, unsigned target);
template void apply_controlled_not(QStateVector<float>& qstate, unsigned control, unsigned target);
template void apply_swap(QStateVector<float>& qstate, unsigned q1, unsigned q2);
template void measure(QStateVector<float>& qstate, unsigned qubit, unsigned& creg);

} // namespace cqq
