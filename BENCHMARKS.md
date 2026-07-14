# CQQ Performance Benchmarks

This document tracks the performance optimizations of the CQQ Quantum Simulator.

## Methodology
The benchmarks are run purely on execution of circuits, no compilation or setup time is included. For details, see the [`benchmarks/bm_simulation.cc`](benchmarks/bm_simulation.cc).

## Running & Adding Benchmarks
To build/run the benchmarks and optionally add a new version under the logs, run:
```bash
chmod +x benchmarks/run_benchmarks.sh
./benchmarks/run_benchmarks.sh
```

## Benchmark Log

## v0.1.0

**Initial Version**
- Creates `std::vector` for simulation state
- Utilizes bit manipulation for gate application
- Uses O3 optimization (inherent in release mode for CMake) and -march=native flags for compilation.

| Benchmark | Qubits | Classical regs | Gates | Shots | Seed | Time | CPU | Iterations |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Random circuit simulation | 8 | 8 | 100 | 1024 | 42 | 79892096 ns (79.9 ms) | 76608444 ns (76.6 ms) | 9 |
| Random circuit simulation | 8 | 8 | 150 | 1024 | 100 | 113680224 ns (113.7 ms) | 109028943 ns (109.0 ms) | 7 |
| Random circuit simulation | 16 | 16 | 100 | 1024 | 1337 | 1.0816e+10 ns (10.8 s) | 1.0402e+10 ns (10.4 s) | 1 |
| Random circuit simulation | 16 | 16 | 150 | 1024 | 5050 | 1.7095e+10 ns (17.1 s) | 1.6462e+10 ns (16.5 s) | 1 |
| Random circuit simulation | 16 | 16 | 200 | 1024 | 413 | 1.8877e+10 ns (18.9 s) | 1.8168e+10 ns (18.2 s) | 1 |



# v0.1.1 Threading investigation

**Hypethesis:** Threading will make the simulation throughput a lot faster.

**Machine:** MacBook Pro, 8 logical CPUs (heterogeneous P/E cores), macOS. Note: Google
Benchmark cannot set thread affinity on macOS, and background load averaged 1.7–3.5
during runs thus adding some minimal noise to the results.


**Method:** Added `->Threads(N)` variants to `BM_Simulation` (16 qubits, 150 gates,
1024 shots), which run independent threads each executing a full simulation
(separate `QuantumSimulator` instance per thread — no shared state).

| Benchmark | Threads | Real time | CPU Time | Efficiency vs linear |
| --- | --- | --- | --- | --- |
| BM_Simulation (16q, 150 gates, 1024 shots) | 1 | 8.349 s | 8.348s | 100% |
| BM_Simulation | 2 | 4.384 s | 8.767s | 95.2% |
| BM_Simulation | 4 | 2.278 s | 9.104s | 91.6% |
| BM_Simulation | 8 | 1.588 s | 12.037s | 65.7% |
| BM_Simulation | 16 | 1.557 s | 12.106s | ~33% |

**Findings:**
- Threading provides near-linear speedup through 4 threads for `BM_Simulation`
  (91.6% efficiency), contradicting the initial hypothesis that threading doesn't help.
- 16 threads on 8 logical cores shows no further gain, as expected from
  oversubscription alone, because the machine doesn't support more than 8 cores, hence adding threads will not speed up computation.


**Next Steps**
-Re-run full suite on a homogeneous-core Linux VM with thread affinity
  (`taskset`) and `perf stat` 
-Add a true memory bound control benchmark like a large array greater than 32mb and exceed the L3 cache size too. 
-Re-run full suite on a homogeneous-core Linux VM with thread affinity
  (`taskset`) and `perf stat` (cache-misses, LLC-load-misses) for a cleaner signal
  than macOS allows.


  