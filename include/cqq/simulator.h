#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "circuit.h"
#include "states.h"

#include <unordered_map>
#include <vector>

namespace cqq {

class QuantumSimulator {
  private:
    unsigned num_qubits;
    QStateVector qstate;

  public:
    QuantumSimulator(unsigned num_qubits);

    std::unordered_map<unsigned, unsigned> execute(const Circuit& circuit, unsigned shots = 1);

    void reset();

    void print_state() const;
};

} // namespace cqq

#endif // SIMULATOR_H
