#ifndef CIRCUIT_GENERATOR_H
#define CIRCUIT_GENERATOR_H

#include "cqq/circuit.h"

#include <algorithm>
#include <random>

namespace cqq::benchmarks {

Circuit generate_random_qasm_circuit(unsigned num_qregs = 16, unsigned num_cregs = 16,
    unsigned num_gates = 100, unsigned seed = 42) {
    Circuit circuit(num_qregs, num_cregs);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<unsigned> q_dist(0, num_qregs - 1);

    // Gate pool: 0=H, 1=X, 2=Y, 3=Z, 4=CX, 5=SWAP
    // Fallback: If num_qregs is 1, prevent it from generating 2-qubit gates
    auto max_gate = (num_qregs > 1) ? 5 : 3;
    std::uniform_int_distribution<int> gate_dist(0, max_gate);

    for (unsigned i = 0; i < num_gates; ++i) {
        int gate_type = gate_dist(rng);
        unsigned target = q_dist(rng);

        switch (gate_type) {
        case 0:
            circuit.add_gate(GateType::H, {target});
            break;
        case 1:
            circuit.add_gate(GateType::X, {target});
            break;
        case 2:
            circuit.add_gate(GateType::Y, {target});
            break;
        case 3:
            circuit.add_gate(GateType::Z, {target});
            break;
        case 4: {
            unsigned control = q_dist(rng);
            while (control == target) {
                control = q_dist(rng);
            }

            circuit.add_gate(GateType::CX, {target}, {control});
            break;
        }
        case 5: {
            unsigned target2 = q_dist(rng);
            while (target2 == target) { // ensure target2 != target
                target2 = q_dist(rng);
            }

            circuit.add_gate(GateType::SWAP, {target, target2});
            break;
        }
        }
    }

    auto num_measurements = std::min(num_qregs, num_cregs);
    for (unsigned i = 0; i < num_measurements; ++i) {
        circuit.add_measurement(i, i);
    }

    return circuit;
}

} // namespace cqq::benchmarks

#endif // CIRCUIT_GENERATOR_H
