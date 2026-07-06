#ifndef CIRCUIT_GENERATOR_H
#define CIRCUIT_GENERATOR_H

#include <string>

namespace cqq::utils {

void generate_random_qasm(
    const std::string& filename, unsigned num_qubits, unsigned num_cregs, unsigned num_gates);

void generate_ghz_qasm(const std::string& filename, unsigned num_qubits, unsigned num_cregs);

} // namespace cqq::utils

#endif // CIRCUIT_GENERATOR_H
