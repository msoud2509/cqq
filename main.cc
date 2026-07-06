#include "include/cqq/circuit.h"
#include "include/cqq/compiler_qasm.h"
#include "include/cqq/simulator.h"

#include <chrono>
#include <iostream>

int main() {
    using namespace cqq;

    QuantumSimulator simulator(10);
    Circuit circuit = CompilerQASM::compile_circuit("sample_circuits/swap_10.qasm");

    // Alternatively, you can create a circuit with operations directly
    // Circuit circuit(2, 2);
    // circuit.add_gate(GateType::H, {0});
    // circuit.add_gate(GateType::H, {1});
    // circuit.add_measurement(0, 0);
    // circuit.add_measurement(1, 1);
    // circuit.print_circuit();

    int num_zero = 0;
    int num_one = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1024; ++i) {
        std::vector<unsigned> result = simulator.execute(circuit, 2);
        if (result[0] == 0) {
            num_zero++;
        } else {
            num_one++;
        }
        simulator.reset();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Execution time for 1000 shots: " << elapsed.count() << std::endl;
    std::cout << "Measured 0: " << num_zero << " times, Measured 1: " << num_one << " times."
              << std::endl;
    return 0;
}
