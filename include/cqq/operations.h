#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "states.h"

namespace cqq {

inline size_t bit_mask(unsigned qubit) {
    return 1ULL << qubit;
}

template <typename Precision> void apply_hadamard(QStateVector<Precision>& qstate, unsigned target);

template <typename Precision> void apply_pauli_x(QStateVector<Precision>& qstate, unsigned target);

template <typename Precision> void apply_pauli_y(QStateVector<Precision>& qstate, unsigned target);

template <typename Precision> void apply_pauli_z(QStateVector<Precision>& qstate, unsigned target);

template <typename Precision>
void apply_controlled_not(QStateVector<Precision>& qstate, unsigned control, unsigned target);

template <typename Precision>
void apply_swap(QStateVector<Precision>& qstate, unsigned q1, unsigned q2);

template <typename Precision>
void measure(QStateVector<Precision>& qstate, unsigned qubit, unsigned& creg);

} // namespace cqq

#endif // OPERATIONS_H