#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>

#include "circuit.h"
#include "states.h"

namespace cqq {

class QuantumSimulator {
  private:
    unsigned num_qubits;
    QStateVector qstate;

  public:
    QuantumSimulator(unsigned num_qubits);

    std::vector<unsigned> execute(const Circuit& circuit, int num_cregs, int shots = 1);

    void reset();

    void print_state() const;
};

} // namespace cqq

#endif // SIMULATOR_H
