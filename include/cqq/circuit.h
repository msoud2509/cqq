#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace cqq {

// enable visitation for std::variant
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

enum class GateType {
    H,    // Hadamard gate
    X,    // Pauli-X gate
    Y,    // Pauli-Y gate
    Z,    // Pauli-Z gate
    CX,   // Controlled-NOT gate
    SWAP, // SWAP gate
};

struct Gate {
    GateType type;
    std::vector<unsigned> targets;
    std::vector<unsigned> controls;
};

struct Measurement {
    unsigned qreg;
    unsigned creg;
};

using Operation = std::variant<Gate, Measurement>;

class Circuit {
  private:
    unsigned num_qregs;
    unsigned num_cregs;

    // The gates/measurements in the circuit, represented as a vector of
    // operations.
    std::vector<Operation> ops;

  public:
    Circuit(unsigned num_qregs, unsigned num_cregs) : num_qregs(num_qregs), num_cregs(num_cregs) {}

    Circuit(unsigned num_qregs, unsigned num_cregs, const std::vector<Operation>& operations)
        : num_qregs(num_qregs), num_cregs(num_cregs), ops(operations) {}

    void add_gate(GateType type, const std::vector<unsigned>& targets,
        const std::vector<unsigned>& controls = {}) {
        ops.push_back(Gate{type, targets, controls});
    }

    void add_measurement(unsigned qreg, unsigned creg) {
        ops.push_back(Measurement{qreg, creg});
    }

    unsigned get_num_qregs() const {
        return num_qregs;
    }
    unsigned get_num_cregs() const {
        return num_cregs;
    }
    const std::vector<Operation>& get_operations() const {
        return ops;
    }

    void print_circuit() const {
        for (const auto& op : ops) {
            auto gate_printer = [](const Gate& gate) {
                std::string gate_name;
                switch (gate.type) {
                case GateType::H:
                    gate_name = "H";
                    break;
                case GateType::X:
                    gate_name = "X";
                    break;
                case GateType::Y:
                    gate_name = "Y";
                    break;
                case GateType::Z:
                    gate_name = "Z";
                    break;
                case GateType::CX:
                    gate_name = "CX";
                    break;
                case GateType::SWAP:
                    gate_name = "SWAP";
                    break;
                }
                std::cout << gate_name << " on qubits ";
                for (unsigned t : gate.targets) {
                    std::cout << t << " ";
                }
                if (!gate.controls.empty()) {
                    std::cout << "with controls ";
                    for (unsigned c : gate.controls) {
                        std::cout << c << " ";
                    }
                }
                std::cout << std::endl;
            };

            auto meas_printer = [](const Measurement& meas) {
                std::cout << "Measure qubit " << meas.qreg << " into classical register "
                          << meas.creg << std::endl;
            };

            std::visit(overloaded{gate_printer, meas_printer}, op);
        }
    }
};

} // namespace cqq

#endif // CIRCUIT_H
