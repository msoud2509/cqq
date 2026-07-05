#ifndef COMPILER_QASM_H
#define COMPILER_QASM_H

#include "circuit.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace cqq {

struct RawInstruction {
    std::string command;
    std::vector<unsigned> registers;
};

class CompilerQASM {
  private:
    static std::string trim(const std::string& str);
    static unsigned extract_index(const std::string& args_str);
    static std::vector<RawInstruction> parse_file(const std::string& filename);
    static Circuit build_circuit(const std::vector<RawInstruction>& instructions);

  public:
    static Circuit compile_circuit(const std::string& filename);
};

} // namespace cqq

#endif // COMPILER_QASM_H
