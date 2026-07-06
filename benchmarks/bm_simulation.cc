#include "circuit_generator.h"
#include "cqq/simulator.h"

#include <benchmark/benchmark.h>

using namespace cqq;

static void BM_Simulation(benchmark::State& state) {
    const unsigned num_qubits = static_cast<unsigned>(state.range(0));
    const unsigned num_cregs = static_cast<unsigned>(state.range(1));
    const unsigned num_gates = static_cast<unsigned>(state.range(2));
    const unsigned num_shots = static_cast<unsigned>(state.range(3));
    const unsigned seed = static_cast<unsigned>(state.range(4));

    Circuit circuit =
        benchmarks::generate_random_qasm_circuit(num_qubits, num_cregs, num_gates, seed);
    QuantumSimulator simulator(num_qubits);

    // actual benchmarking loop
    for (auto _ : state) {
        std::unordered_map<unsigned, unsigned> results = simulator.execute(circuit, num_shots);
        benchmark::DoNotOptimize(results);
        simulator.reset();
    }
}

BENCHMARK(BM_Simulation)
    ->Args({8,  8,  100, 1024, 42})
    ->Args({8,  8,  150, 1024, 100})
    ->Args({16, 16, 100, 1024, 1337})
    ->Args({16, 16, 150, 1024, 5050})
    ->Args({16, 16, 200, 1024, 413});
