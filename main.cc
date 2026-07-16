#include "include/cqq/circuit.h"
#include "include/cqq/compiler_qasm.h"
#include "include/cqq/simulator.h"

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

constexpr unsigned NUM_SHOTS = 1024;

int main() {
    using namespace cqq;

    QuantumSimulator simulator(2);
    Circuit circuit = CompilerQASM::compile_circuit("sample_circuits/bell_state.qasm");

    // Alternatively, you can create a circuit with build operations directly
    // Circuit circuit(2, 2);
    // circuit.add_gate(GateType::H, {0});
    // circuit.add_gate(GateType::H, {1});
    // circuit.add_measurement(0, 0);
    // circuit.add_measurement(1, 1);
    // circuit.print_circuit();

    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = simulator.execute(circuit, NUM_SHOTS);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Execution time for " << NUM_SHOTS << " shots: " << elapsed.count() << std::endl;

    std::cout << "Measurement histogram:" << std::endl;
    for (const auto& [outcome, count] : result) {
        std::string bits(circuit.get_num_cregs(), '0');
        for (unsigned i = 0; i < circuit.get_num_cregs(); ++i) {
            if ((outcome >> i) & 1u) {
                bits[circuit.get_num_cregs() - 1 - i] = '1';
            }
        }

        std::cout << "  0b" << bits << ": " << count << std::endl;
    }
    return 0;
}
