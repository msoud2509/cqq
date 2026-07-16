#include "cqq/circuit.h"

#include <sstream>
#include <string>

#include <gtest/gtest.h>

TEST(CircuitUnit, ConstructorSetsRegisterCounts) {
    const cqq::Circuit circuit(3, 2);

    EXPECT_EQ(circuit.get_num_qregs(), 3u);
    EXPECT_EQ(circuit.get_num_cregs(), 2u);
    EXPECT_TRUE(circuit.get_operations().empty());
}

TEST(CircuitUnit, AddGateAndMeasurementPreservesOrderAndValues) {
    cqq::Circuit circuit(2, 2);
    circuit.add_gate(cqq::GateType::H, {0});
    circuit.add_gate(cqq::GateType::CX, {1}, {0});
    circuit.add_measurement(1, 1);

    const auto& ops = circuit.get_operations();
    ASSERT_EQ(ops.size(), 3u);

    const auto* h_gate = std::get_if<cqq::Gate>(&ops[0]);
    ASSERT_NE(h_gate, nullptr);
    EXPECT_EQ(h_gate->type, cqq::GateType::H);
    EXPECT_EQ(h_gate->targets, (std::vector<unsigned>{0}));
    EXPECT_TRUE(h_gate->controls.empty());

    const auto* cx_gate = std::get_if<cqq::Gate>(&ops[1]);
    ASSERT_NE(cx_gate, nullptr);
    EXPECT_EQ(cx_gate->type, cqq::GateType::CX);
    EXPECT_EQ(cx_gate->targets, (std::vector<unsigned>{1}));
    EXPECT_EQ(cx_gate->controls, (std::vector<unsigned>{0}));

    const auto* meas = std::get_if<cqq::Measurement>(&ops[2]);
    ASSERT_NE(meas, nullptr);
    EXPECT_EQ(meas->qreg, 1u);
    EXPECT_EQ(meas->creg, 1u);
}

TEST(CircuitUnit, PrintCircuitIncludesGateAndMeasurementLines) {
    cqq::Circuit circuit(2, 1);
    circuit.add_gate(cqq::GateType::X, {0});
    circuit.add_measurement(0, 0);

    std::ostringstream capture;
    auto* old_buf = std::cout.rdbuf(capture.rdbuf());
    circuit.print_circuit();
    std::cout.rdbuf(old_buf);

    const std::string output = capture.str();
    EXPECT_NE(output.find("X on qubits 0"), std::string::npos);
    EXPECT_NE(output.find("Measure qubit 0 into classical register 0"), std::string::npos);
}
