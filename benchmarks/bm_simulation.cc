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
        benchmark::ClobberMemory();
        simulator.reset();
    }
}

// while 1024 shots are standard for quantum computing experiments, we are using codspeed for
// benchmarking, so this drastically reduced the simulation time
BENCHMARK(BM_Simulation)
    ->Args({12, 12, 100, 1, 42})
    ->Args({12, 12, 150, 1, 100})
    ->Args({16, 16, 100, 1, 1337})
    ->Args({18, 18, 100, 1, 5050})
    ->Args({20, 20, 100, 1, 413});

BENCHMARK_MAIN();
