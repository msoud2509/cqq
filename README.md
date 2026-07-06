# CQQ

A C++(20) project for compiling and simulating quantum circuits. 

## Usage
To create the build system, run:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

To compile and run the project, use the following commands:
```bash
cmake --build build
./build/cqq
```

To format the code, run:
```bash
cmake --build build --target format
```
