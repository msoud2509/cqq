#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "states.h"

namespace cqq {

inline size_t bit_mask(unsigned qubit) {
    return 1ULL << qubit;
}

void apply_hadamard(QStateVector& qstate, unsigned target);
void apply_pauli_x(QStateVector& qstate, unsigned target);
void apply_pauli_y(QStateVector& qstate, unsigned target);
void apply_pauli_z(QStateVector& qstate, unsigned target);
void apply_controlled_not(QStateVector& qstate, unsigned control, unsigned target);
void apply_swap(QStateVector& qstate, unsigned q1, unsigned q2);

void measure(QStateVector& qstate, unsigned qubit, unsigned& creg);

} // namespace cqq

#endif // OPERATIONS_H