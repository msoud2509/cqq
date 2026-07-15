#ifndef STATES_H
#define STATES_H

#include <bit>
#include <complex>
#include <vector>

namespace cqq {

using QStateVector = std::vector<std::complex<double>>;

inline unsigned num_qubits(const QStateVector& qstate) {
    return static_cast<unsigned>(std::countr_zero(qstate.size()));
}

} // namespace cqq

#endif // STATES_H
