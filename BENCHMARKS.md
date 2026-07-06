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
