#ifndef STATES_H
#define STATES_H

#include <bit>
#include <complex>
#include <vector>

namespace cqq {

template <typename Precision = double> using QStateVector = std::vector<std::complex<Precision>>;

template <typename Precision = double>
inline unsigned num_qubits(const QStateVector<Precision>& qstate) {
    return static_cast<unsigned>(std::countr_zero(qstate.size()));
}

} // namespace cqq

#endif // STATES_H
