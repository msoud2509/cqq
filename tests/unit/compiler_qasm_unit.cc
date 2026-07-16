#include "cqq/compiler_qasm.h"

#include <filesystem>
#include <fstream>
#include <string>

#include <gtest/gtest.h>

namespace {

std::filesystem::path write_temp_qasm_file(const std::string& content) {
    const auto file_path =
        std::filesystem::temp_directory_path() /
        ("cqq_test_" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()) + "_" +
            std::to_string(std::rand()) + ".qasm");

    std::ofstream out(file_path);
    out << content;
    out.close();

    return file_path;
}

} // namespace

TEST(CompilerQASMUnit, CompilesValidProgramToCircuit) {
    const auto path = write_temp_qasm_file("OPENQASM 2.0;\n"
                                           "include \"qelib1.inc\";\n"
                                           "qreg q[2];\n"
                                           "creg c[2];\n"
                                           "h q[0];\n"
                                           "cx q[0] q[1];\n"
                                           "measure q[0] -> c[0];\n"
                                           "measure q[1] -> c[1];\n");

    const auto circuit = cqq::CompilerQASM::compile_circuit(path.string());
    std::filesystem::remove(path);

    EXPECT_EQ(circuit.get_num_qregs(), 2u);
    EXPECT_EQ(circuit.get_num_cregs(), 2u);

    const auto& ops = circuit.get_operations();
    ASSERT_EQ(ops.size(), 4u);
    EXPECT_TRUE(std::holds_alternative<cqq::Gate>(ops[0]));
    EXPECT_TRUE(std::holds_alternative<cqq::Gate>(ops[1]));
    EXPECT_TRUE(std::holds_alternative<cqq::Measurement>(ops[2]));
    EXPECT_TRUE(std::holds_alternative<cqq::Measurement>(ops[3]));
}

TEST(CompilerQASMUnit, ThrowsForMissingFile) {
    EXPECT_THROW(cqq::CompilerQASM::compile_circuit("/definitely/not/found/program.qasm"),
        std::runtime_error);
}

TEST(CompilerQASMUnit, ThrowsOnUnknownInstruction) {
    const auto path = write_temp_qasm_file("qreg q[1];\n"
                                           "creg c[1];\n"
                                           "foo q[0];\n");

    EXPECT_THROW(cqq::CompilerQASM::compile_circuit(path.string()), std::runtime_error);
    std::filesystem::remove(path);
}

TEST(CompilerQASMUnit, ThrowsOnIncompleteStatementAtEndOfFile) {
    const auto path = write_temp_qasm_file("qreg q[1];\n"
                                           "creg c[1];\n"
                                           "h q[0]");

    EXPECT_THROW(cqq::CompilerQASM::compile_circuit(path.string()), std::runtime_error);
    std::filesystem::remove(path);
}
