# CQQ

A C++(20) project for compiling and simulating quantum circuits. 

## Usage
To create the build system, run:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

To compile and run the project, use the following commands:
```bash
cmake --build build --target cqq
./build/cqq
```

To format the code, run:
```bash
cmake --build build --target format
```

## Tests
The project includes unit and end-to-end tests built with GoogleTest. After configuring the build
directory, build the test target and run the test suite with CTest:
```bash
cmake --build build --target cqq_unit_tests cqq_e2e_tests
ctest --test-dir build --output-on-failure
```

You can also run the test binary directly:
```bash
./build/cqq_e2e_tests && ./build/cqq_unit_tests
```

## Performance Benchmarking
This project uses CodSpeed to track performance, to setup CodSpeed:

1. Install the [CodSpeed CLI](https://codspeed.io/docs/cli) 

2. Create a free [CodSpeed account](https://app.codspeed.io/login?flow=get-started)

3. To authenticate with the CLI, run:
```bash
codspeed auth login
```

Then to run the benchmarks, build the benchmark target and run the binary:
```bash
cmake --build build --target cqq_benchmarks
codspeed exec --mode simulation ./build/cqq_benchmarks
```
