#include "../include/cqq/compiler_qasm.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <optional>

namespace cqq {

std::string CompilerQASM::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

unsigned CompilerQASM::extract_index(const std::string& args_str) {
    auto start = args_str.find('[');
    auto end = args_str.find(']');

    if (start == std::string::npos || end == std::string::npos || end <= start) {
        throw std::runtime_error("Invalid register format: " + args_str);
    }

    std::string index_str = args_str.substr(start + 1, end - start - 1);
    std::stringstream ss(index_str);
    unsigned index;
    if (!(ss >> index)) {
        throw std::runtime_error("Failed to parse index in: " + args_str);
    }

    return index;
}

std::optional<RawInstruction> CompilerQASM::parse_statement(const std::string stmt) {
    if (stmt.empty()) {
        return std::nullopt;
    }

    // Skip headers/includes for basic simulation
    if (stmt.rfind("OPENQASM", 0) == 0 || stmt.rfind("include", 0) == 0) {
        return std::nullopt; 
    }

    std::stringstream ss(stmt);
    std::string command;
    ss >> command;

    std::vector<unsigned> operands;
    std::string operand;
    while (ss >> operand) {
        if (operand == "->") {
            continue;
        }
        operands.push_back(extract_index(operand));
    }

    return RawInstruction{command, operands};
}

std::vector<RawInstruction> CompilerQASM::parse_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open QASM file: " + filename);
    }

    std::vector<RawInstruction> instructions;
    std::string line;
    std::string buffer;
    unsigned line_number = 0;

    try {
        while (std::getline(file, line)) {
            ++line_number;

            // strip comments
            auto comment_pos = line.find("//");
            if (comment_pos != std::string::npos) {
                line = line.substr(0, comment_pos);
            }

            buffer = trim(line);

            size_t semicolon_pos;
            while ((semicolon_pos = buffer.find(';')) != std::string::npos) {
                std::string stmt = buffer.substr(0, semicolon_pos);
                stmt = trim(stmt);

                if (auto instr = parse_statement(stmt)) {
                    instructions.push_back(*instr);
                }

                buffer = buffer.substr(semicolon_pos + 1);
            }
        }

        if (!trim(buffer).empty()) {
            throw std::runtime_error("Incomplete statement at end of file.");
        }

    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Error parsing QASM file at line " + std::to_string(line_number) + ": " + e.what());
    }

    return instructions;
}

Circuit CompilerQASM::build_circuit(const std::vector<RawInstruction>& instructions) {
    unsigned num_qregs = 0;
    unsigned num_cregs = 0;
    std::vector<Operation> operations;

    for (const auto& instr : instructions) {
        if (instr.command == "qreg")
            num_qregs = instr.registers[0];
        else if (instr.command == "creg")
            num_cregs = instr.registers[0];
        else if (instr.command == "h")
            operations.push_back(Gate{GateType::H, {instr.registers[0]}, {}});
        else if (instr.command == "x")
            operations.push_back(Gate{GateType::X, {instr.registers[0]}, {}});
        else if (instr.command == "y")
            operations.push_back(Gate{GateType::Y, {instr.registers[0]}, {}});
        else if (instr.command == "z")
            operations.push_back(Gate{GateType::Z, {instr.registers[0]}, {}});
        else if (instr.command == "cx")
            operations.push_back(Gate{GateType::CX, {instr.registers[1]}, {instr.registers[0]}});
        else if (instr.command == "swap")
            operations.push_back(
                Gate{GateType::SWAP, {instr.registers[0], instr.registers[1]}, {}});
        else if (instr.command == "measure")
            operations.push_back(Measurement{instr.registers[0], instr.registers[1]});
        else
            throw std::runtime_error("Unknown command in QASM: " + instr.command);
    }

    return Circuit(num_qregs, num_cregs, operations);
}

Circuit CompilerQASM::compile_circuit(const std::string& filename) {
    auto instructions = parse_file(filename);
    return build_circuit(instructions);
}

} // namespace cqq
