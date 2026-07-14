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
./build/cqq_e2e_tests
```

**NOTE:** To see benchmark usage and results, see [`BENCHMARKS.md`](BENCHMARKS.md).
