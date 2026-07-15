#include "../include/cqq/simulator.h"
#include "../include/cqq/operations.h"

#include <algorithm>
#include <complex>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace cqq {

QuantumSimulator::QuantumSimulator(unsigned num_qubits)
    : num_qubits(num_qubits), qstate(1ULL << num_qubits) {
    reset();
}

std::unordered_map<unsigned, unsigned> QuantumSimulator::execute(
    const Circuit& circuit, unsigned shots) {
    if (circuit.get_num_qregs() > num_qubits) {
        throw std::invalid_argument(
            "Circuit requires more qubits than available in the simulator.");
    }

    const unsigned num_cregs = circuit.get_num_cregs();
    std::unordered_map<unsigned, unsigned> measurement_counts;
    for (unsigned shot = 0; shot < shots; ++shot) {
        std::vector<unsigned> cregs(num_cregs, 0);

        for (const Operation& op : circuit.get_operations()) {
            auto gate_processor = [this](const Gate& gate) {
                switch (gate.type) {
                case GateType::H:
                    apply_hadamard(qstate, gate.targets[0]);
                    break;
                case GateType::X:
                    apply_pauli_x(qstate, gate.targets[0]);
                    break;
                case GateType::Y:
                    apply_pauli_y(qstate, gate.targets[0]);
                    break;
                case GateType::Z:
                    apply_pauli_z(qstate, gate.targets[0]);
                    break;
                case GateType::CX:
                    apply_controlled_not(qstate, gate.controls[0], gate.targets[0]);
                    break;
                case GateType::SWAP:
                    apply_swap(qstate, gate.targets[0], gate.targets[1]);
                    break;
                default:
                    throw std::invalid_argument("Error: Unknown gate type.");
                }
            };

            auto measurement_processor = [this, &cregs](const Measurement& meas) {
                measure(qstate, meas.qreg, cregs[meas.creg]);
            };

            std::visit(overloaded{gate_processor, measurement_processor}, op);
        }

        unsigned measurement_result = 0;
        for (unsigned i = 0; i < cregs.size(); ++i) {
            measurement_result |= (cregs[i] << i);
        }
        measurement_counts[measurement_result]++;

        reset(); // Reset the quantum state for the next shot
    }

    return measurement_counts;
}

void QuantumSimulator::reset() {
    std::fill(qstate.begin(), qstate.end(), std::complex<double>(0.0, 0.0));
    qstate[0] = {1.0, 0.0};
}

void QuantumSimulator::print_state() const {
    for (size_t i = 0; i < qstate.size(); ++i) {
        std::string bits;
        bits.reserve(num_qubits);

        for (int b = num_qubits - 1; b >= 0; --b) {
            bits += ((i >> b) & 1) ? '1' : '0';
        }
        std::cout << "|" << bits << ">: " << qstate[i] << "\n";
    }
}

} // namespace cqq
