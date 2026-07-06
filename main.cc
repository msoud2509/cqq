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

    QuantumSimulator simulator(10);
    Circuit circuit = CompilerQASM::compile_circuit("sample_circuits/ghz_10.qasm");

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
    
    // convert the measurement results to bitstrings and print them
    const unsigned num_bits = circuit.get_num_cregs();
    for (const auto& [measurement, count] : result) {
        std::string bitstring(num_bits, '0');
        for (unsigned bit = 0; bit < num_bits; ++bit) {
            if (measurement & (1U << bit)) {
                bitstring[num_bits - 1 - bit] = '1';
            }
        }
        std::cout << "Measured " << bitstring << " (" << measurement << "): " << count << " times."
                  << std::endl;
    }
    return 0;
}
