#include "cqq/compiler_qasm.h"
#include "cqq/simulator.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

namespace {

std::filesystem::path sample_circuit_path(const std::string& filename) {
    return std::filesystem::path(CQQ_SAMPLE_CIRCUITS_DIR) / filename;
}

cqq::Circuit compile_sample_circuit(const std::string& filename) {
    return cqq::CompilerQASM::compile_circuit(sample_circuit_path(filename).string());
}

unsigned total_measurements(const std::unordered_map<unsigned, unsigned>& counts) {
    unsigned total = 0;
    for (const auto& [outcome, count] : counts) {
        (void)outcome;
        total += count;
    }
    return total;
}

} // namespace

TEST(CircuitE2E, CompilesAllSampleCircuits) {
    const std::vector<std::string> samples = {
        "bell_state.qasm", "ghz_10.qasm", "swap_10.qasm", "hardware_ansatz_12.qasm"};

    for (const auto& sample : samples) {
        SCOPED_TRACE(sample);

        const auto circuit = compile_sample_circuit(sample);
        EXPECT_GT(circuit.get_num_qregs(), 0u);
        EXPECT_GT(circuit.get_num_cregs(), 0u);
        EXPECT_FALSE(circuit.get_operations().empty());
    }
}

TEST(CircuitE2E, BellStateOnlyProducesBellOutcomes) {
    const auto circuit = compile_sample_circuit("bell_state.qasm");
    ASSERT_EQ(circuit.get_num_qregs(), 2u);
    ASSERT_EQ(circuit.get_num_cregs(), 2u);

    cqq::QuantumSimulator simulator(circuit.get_num_qregs());
    const auto counts = simulator.execute(circuit, 256);

    EXPECT_EQ(total_measurements(counts), 256u);
    for (const auto& [outcome, count] : counts) {
        (void)count;
        EXPECT_TRUE(outcome == 0u || outcome == 3u);
    }
}

TEST(CircuitE2E, GhzOnlyProducesAllZeroOrAllOne) {
    const auto circuit = compile_sample_circuit("ghz_10.qasm");
    ASSERT_EQ(circuit.get_num_qregs(), 10u);
    ASSERT_EQ(circuit.get_num_cregs(), 10u);

    cqq::QuantumSimulator simulator(circuit.get_num_qregs());
    const auto counts = simulator.execute(circuit, 256);

    EXPECT_EQ(total_measurements(counts), 256u);
    for (const auto& [outcome, count] : counts) {
        (void)count;
        EXPECT_TRUE(outcome == 0u || outcome == 1023u);
    }
}

TEST(CircuitE2E, RemainingSampleCircuitsCompileAndRun) {
    const std::vector<std::string> samples = {"swap_10.qasm", "hardware_ansatz_12.qasm"};

    for (const auto& sample : samples) {
        SCOPED_TRACE(sample);

        const auto circuit = compile_sample_circuit(sample);
        cqq::QuantumSimulator simulator(circuit.get_num_qregs());
        const auto counts = simulator.execute(circuit, 64);

        EXPECT_EQ(total_measurements(counts), 64u);
        EXPECT_FALSE(counts.empty());
    }
}
