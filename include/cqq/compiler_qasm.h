#ifndef COMPILER_QASM_H
#define COMPILER_QASM_H

#include "circuit.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>

namespace cqq {

struct RawInstruction {
    std::string command;
    std::vector<unsigned> registers;
};

class CompilerQASM {
  private:
    // Trims whitespace from the beginning and end of a string.
    static std::string trim(const std::string& str);

    // Extracts the index from a string representing a register.
    static unsigned extract_index(const std::string& args_str);

    // Parses a single statements passed to it by the parse_file function and returns a RawInstruction object.
    static std::optional<RawInstruction> parse_statement(const std::string stmt);

    // Parses a qasm file and returns a vector of raw instructions.
    static std::vector<RawInstruction> parse_file(const std::string& filename);

    // builds a Circuit object from a vector of raw instructions.
    static Circuit build_circuit(const std::vector<RawInstruction>& instructions);

  public:
    // Parses a qasm file and builds then returns a Circuit object representing the quantum circuit.
    static Circuit compile_circuit(const std::string& filename);
};

} // namespace cqq

#endif // COMPILER_QASM_H
