#include "circuit_generator.h"
#include "cqq/simulator.h"

#include <benchmark/benchmark.h>

using namespace cqq;

template <typename Precision> static void BM_Simulation(benchmark::State& state) {
    const unsigned num_qubits = static_cast<unsigned>(state.range(0));
    const unsigned num_cregs = static_cast<unsigned>(state.range(1));
    const unsigned num_gates = static_cast<unsigned>(state.range(2));
    const unsigned num_shots = static_cast<unsigned>(state.range(3));
    const unsigned seed = static_cast<unsigned>(state.range(4));

    Circuit circuit =
        benchmarks::generate_random_qasm_circuit(num_qubits, num_cregs, num_gates, seed);
    QuantumSimulator<Precision> simulator(num_qubits);

    // actual benchmarking loop
    for (auto _ : state) {
        std::unordered_map<unsigned, unsigned> results = simulator.execute(circuit, num_shots);
        benchmark::DoNotOptimize(results);
        benchmark::ClobberMemory();
        simulator.reset();
    }
}

// while 1024 shots are standard for quantum computing experiments, we are using codspeed for
// benchmarking which makes consistent simulations, so this drastically reduced the simulation time
BENCHMARK_TEMPLATE(BM_Simulation, float)
    ->Args({12, 12, 200, 1, 102})
    ->Args({12, 12, 250, 1, 103})
    ->Args({12, 12, 300, 1, 104})
    ->Args({16, 16, 200, 1, 105})
    ->Args({16, 16, 250, 1, 106})
    ->Args({16, 16, 300, 1, 107})
    ->Args({18, 18, 100, 1, 108})
    ->Args({18, 18, 150, 1, 109})
    ->Args({18, 18, 200, 1, 110})
    ->Args({20, 20, 50, 1, 111})
    ->Args({22, 22, 20, 1, 112});

BENCHMARK_TEMPLATE(BM_Simulation, double)
    ->Args({8, 8, 200, 1, 115})
    ->Args({8, 8, 250, 1, 116})
    ->Args({8, 8, 300, 1, 117})
    ->Args({12, 12, 200, 1, 118})
    ->Args({12, 12, 250, 1, 119})
    ->Args({12, 12, 300, 1, 120})
    ->Args({16, 16, 100, 1, 120})
    ->Args({16, 16, 150, 1, 120})
    ->Args({16, 16, 200, 1, 121})
    ->Args({18, 18, 50, 1, 122})
    ->Args({20, 20, 20, 1, 123});

BENCHMARK_MAIN();
