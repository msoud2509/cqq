#include "../include/cqq/simulator.h"

#include "../include/cqq/operations.h"

#include <algorithm>
#include <complex>
#include <iostream>
#include <stdexcept>

namespace cqq {

QuantumSimulator::QuantumSimulator(unsigned num_qubits)
    : num_qubits(num_qubits), qstate(1ULL << num_qubits) {
    reset();
}

std::vector<unsigned> QuantumSimulator::execute(const Circuit& circuit, int num_cregs, int shots) {
    if (circuit.get_num_qregs() > num_qubits) {
        throw std::invalid_argument(
            "Circuit requires more qubits than available in the simulator.");
    }

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

    return cregs;
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
