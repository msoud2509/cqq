#include "cqq/simulator.h"

#include <unordered_map>

#include <gtest/gtest.h>

namespace {

unsigned total_counts(const std::unordered_map<unsigned, unsigned>& counts) {
    unsigned total = 0;
    for (const auto& [outcome, count] : counts) {
        (void)outcome;
        total += count;
    }
    return total;
}

} // namespace

TEST(SimulatorUnit, ExecuteSingleMeasurementOnZeroState) {
    cqq::Circuit circuit(1, 1);
    circuit.add_measurement(0, 0);

    cqq::QuantumSimulator simulator(1);
    const auto counts = simulator.execute(circuit, 32);

    ASSERT_EQ(total_counts(counts), 32u);
    ASSERT_EQ(counts.size(), 1u);
    EXPECT_EQ(counts.at(0u), 32u);
}

TEST(SimulatorUnit, ExecuteXThenMeasureAlwaysReturnsOne) {
    cqq::Circuit circuit(1, 1);
    circuit.add_gate(cqq::GateType::X, {0});
    circuit.add_measurement(0, 0);

    cqq::QuantumSimulator simulator(1);
    const auto counts = simulator.execute(circuit, 32);

    ASSERT_EQ(total_counts(counts), 32u);
    ASSERT_EQ(counts.size(), 1u);
    EXPECT_EQ(counts.at(1u), 32u);
}

TEST(SimulatorUnit, ExecuteHadamardThenMeasureProducesValidOutcomes) {
    cqq::Circuit circuit(1, 1);
    circuit.add_gate(cqq::GateType::H, {0});
    circuit.add_measurement(0, 0);

    cqq::QuantumSimulator simulator(1);
    const auto counts = simulator.execute(circuit, 64);

    EXPECT_EQ(total_counts(counts), 64u);
    for (const auto& [outcome, count] : counts) {
        (void)count;
        EXPECT_TRUE(outcome == 0u || outcome == 1u);
    }
}

TEST(SimulatorUnit, ThrowsWhenCircuitNeedsMoreQubitsThanSimulator) {
    cqq::Circuit circuit(2, 1);
    circuit.add_measurement(0, 0);

    cqq::QuantumSimulator simulator(1);
    EXPECT_THROW(simulator.execute(circuit, 1), std::invalid_argument);
}

TEST(SimulatorUnit, ResetRestoresZeroStateAfterXExecution) {
    cqq::Circuit prepare_one_and_measure(1, 1);
    prepare_one_and_measure.add_gate(cqq::GateType::X, {0});
    prepare_one_and_measure.add_measurement(0, 0);

    cqq::Circuit measure_only(1, 1);
    measure_only.add_measurement(0, 0);

    cqq::QuantumSimulator simulator(1);
    const auto one_counts = simulator.execute(prepare_one_and_measure, 1);
    ASSERT_EQ(one_counts.size(), 1u);
    EXPECT_EQ(one_counts.at(1u), 1u);

    simulator.reset();

    const auto zero_counts = simulator.execute(measure_only, 16);
    ASSERT_EQ(zero_counts.size(), 1u);
    EXPECT_EQ(zero_counts.at(0u), 16u);
}

TEST(SimulatorUnit, ResetRestoresZeroStateAfterSuperpositionExecution) {
    cqq::Circuit hadamard_then_measure(1, 1);
    hadamard_then_measure.add_gate(cqq::GateType::H, {0});
    hadamard_then_measure.add_measurement(0, 0);

    cqq::Circuit measure_only(1, 1);
    measure_only.add_measurement(0, 0);

    cqq::QuantumSimulator simulator(1);
    const auto mixed_counts = simulator.execute(hadamard_then_measure, 32);
    ASSERT_EQ(total_counts(mixed_counts), 32u);

    simulator.reset();

    const auto zero_counts = simulator.execute(measure_only, 32);
    ASSERT_EQ(zero_counts.size(), 1u);
    EXPECT_EQ(zero_counts.at(0u), 32u);
}
