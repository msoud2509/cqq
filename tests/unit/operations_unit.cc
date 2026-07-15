#include "cqq/operations.h"

#include <cmath>
#include <complex>

#include <gtest/gtest.h>

namespace {

cqq::QStateVector basis_state(size_t num_qubits, size_t basis_index) {
    cqq::QStateVector state(1ULL << num_qubits, std::complex<double>(0.0, 0.0));
    state[basis_index] = std::complex<double>(1.0, 0.0);
    return state;
}

void expect_state_near(
    const cqq::QStateVector& actual, const cqq::QStateVector& expected, double tolerance = 1e-12) {
    ASSERT_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < actual.size(); ++i) {
        EXPECT_NEAR(actual[i].real(), expected[i].real(), tolerance) << "at index " << i;
        EXPECT_NEAR(actual[i].imag(), expected[i].imag(), tolerance) << "at index " << i;
    }
}

} // namespace

////////////////////////////////
// Hadamard Gate Tests
////////////////////////////////
TEST(OperationsUnit, HadamardMapsZeroToPlusState) {
    auto state = basis_state(1, 0);

    cqq::apply_hadamard(state, 0);

    const auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    const cqq::QStateVector expected = {
        std::complex<double>(inv_sqrt2, 0.0), std::complex<double>(inv_sqrt2, 0.0)};

    expect_state_near(state, expected);
}

TEST(OperationsUnit, HadamardIsItsOwnInverse) {
    auto state = basis_state(1, 0);

    cqq::apply_hadamard(state, 0);
    cqq::apply_hadamard(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

TEST(OperationsUnit, HadamardOnTwoQubits) {
    auto state = basis_state(2, 0);

    cqq::apply_hadamard(state, 0);
    cqq::apply_hadamard(state, 1);

    const auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    const cqq::QStateVector expected = {std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0),
        std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0),
        std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0),
        std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0)};

    expect_state_near(state, expected);
}

TEST(OperationsUnit, HadamardOnOneQubitInTwoQubitSystem) {
    auto state = basis_state(2, 0);

    cqq::apply_hadamard(state, 1);

    const auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    const cqq::QStateVector expected = {std::complex<double>(inv_sqrt2, 0.0),
        std::complex<double>(0.0, 0.0), std::complex<double>(inv_sqrt2, 0.0),
        std::complex<double>(0.0, 0.0)};

    expect_state_near(state, expected);
}

////////////////////////////////
// Pauli-X Gate Tests
////////////////////////////////
TEST(OperationsUnit, PauliXMapsZeroToOne) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_x(state, 0);

    expect_state_near(state, basis_state(1, 1));
}

TEST(OperationsUnit, PauliXIsItsOwnInverse) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_x(state, 0);
    cqq::apply_pauli_x(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

TEST(OperationsUnit, PauliXOnTwoQubits) {
    auto state = basis_state(2, 0);

    cqq::apply_pauli_x(state, 0);
    cqq::apply_pauli_x(state, 1);

    expect_state_near(state, basis_state(2, 3));
}

////////////////////////////////
// Pauli-Z Gate Tests
////////////////////////////////
TEST(OperationsUnit, PauliZMapsOneToNegativeOne) {
    auto state = basis_state(1, 1);

    cqq::apply_pauli_z(state, 0);

    const cqq::QStateVector expected = {
        std::complex<double>(0.0, 0.0), std::complex<double>(-1.0, 0.0)};
    expect_state_near(state, expected);
}

TEST(OperationsUnit, PauliZDoesNotAffectZero) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_z(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

TEST(OperationsUnit, PauliZIsItsOwnInverse) {
    auto state = basis_state(1, 1);

    cqq::apply_pauli_z(state, 0);
    cqq::apply_pauli_z(state, 0);

    expect_state_near(state, basis_state(1, 1));
}

TEST(OperationsUnit, PauliZOnTwoQubits) {
    auto state = basis_state(2, 3);

    cqq::apply_pauli_z(state, 0);
    cqq::apply_pauli_z(state, 1);

    const cqq::QStateVector expected = {std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
        std::complex<double>(1.0, 0.0)};
    expect_state_near(state, expected);
}

////////////////////////////////
// Pauli-Y Gate Tests
////////////////////////////////
TEST(OperationsUnit, PauliYMapsZeroToI) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_y(state, 0);

    const cqq::QStateVector expected = {
        std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 1.0)};
    expect_state_near(state, expected);
}

TEST(OperationsUnit, PauliYMapsOneToNegativeI) {
    auto state = basis_state(1, 1);

    cqq::apply_pauli_y(state, 0);

    const cqq::QStateVector expected = {
        std::complex<double>(0.0, -1.0), std::complex<double>(0.0, 0.0)};
    expect_state_near(state, expected);
}

TEST(OperationsUnit, PauliYIsItsOwnInverse) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_y(state, 0);
    cqq::apply_pauli_y(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

////////////////////////////////
// SWAP Gate Tests
////////////////////////////////
TEST(OperationsUnit, SwapSwapsTwoQubits) {
    auto state = basis_state(2, 1); // |01>

    cqq::apply_swap(state, 0, 1);

    expect_state_near(state, basis_state(2, 2)); // |10>
}

TEST(OperationsUnit, SwapIsItsOwnInverse) {
    auto state = basis_state(2, 1); // |01>

    cqq::apply_swap(state, 0, 1);
    cqq::apply_swap(state, 0, 1);

    expect_state_near(state, basis_state(2, 1)); // |01>
}

TEST(OperationsUnit, SwapOnTwoQubits) {
    auto state = basis_state(2, 0); // |00>

    cqq::apply_swap(state, 0, 1);

    expect_state_near(state, basis_state(2, 0)); // |00>
}

TEST(OperationsUnit, SwapOnThreeQubits) {
    auto state = basis_state(3, 1); // |001>

    cqq::apply_swap(state, 0, 2);

    expect_state_near(state, basis_state(3, 4)); // |100>
}

////////////////////////////////
// CNOT Gate Tests
////////////////////////////////
TEST(OperationsUnit, CNOTFlipsTargetWhenControlIsOne) {
    auto state = basis_state(2, 3); // |11>

    cqq::apply_controlled_not(state, 0, 1);

    expect_state_near(state, basis_state(2, 1)); // |10>
}

TEST(OperationsUnit, CNOTDoesNotFlipTargetWhenControlIsZero) {
    auto state = basis_state(2, 1); // |01>

    cqq::apply_controlled_not(state, 1, 0);

    expect_state_near(state, basis_state(2, 1)); // |01>
}

TEST(OperationsUnit, CNOTIsItsOwnInverse) {
    auto state = basis_state(2, 3); // |11>

    cqq::apply_controlled_not(state, 0, 1);
    cqq::apply_controlled_not(state, 0, 1);

    expect_state_near(state, basis_state(2, 3)); // |11>
}

////////////////////////////////
// Measurement Tests
////////////////////////////////
TEST(OperationsUnit, MeasureCollapsesStateToBasisState) {
    auto state = basis_state(1, 0);
    unsigned creg;

    cqq::measure(state, 0, creg);

    EXPECT_EQ(creg, 0);
    expect_state_near(state, basis_state(1, 0));
}

TEST(OperationsUnit, MeasureCollapsesSuperpositionToBasisState) {
    cqq::QStateVector state = {std::complex<double>(1.0 / std::sqrt(2.0), 0.0),
        std::complex<double>(1.0 / std::sqrt(2.0), 0.0)};
    unsigned creg;

    cqq::measure(state, 0, creg);

    EXPECT_TRUE(creg == 0 || creg == 1);
    if (creg == 0) {
        expect_state_near(state, basis_state(1, 0));
    } else {
        expect_state_near(state, basis_state(1, 1));
    }
}

TEST(OperationsUnit, MeasureThrowsOnInvalidQubitIndex) {
    auto state = basis_state(1, 0);
    unsigned creg;

    EXPECT_THROW(cqq::measure(state, 1, creg), std::invalid_argument);
}

TEST(OperationsUnit, MeasureOnTwoQubits) {
    cqq::QStateVector state = {std::complex<double>(1.0 / std::sqrt(2.0), 0.0),
        std::complex<double>(1.0 / std::sqrt(2.0), 0.0), std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0)};
    unsigned creg;

    cqq::measure(state, 0, creg);

    EXPECT_TRUE(creg == 0 || creg == 1);
    if (creg == 0) {
        expect_state_near(state, basis_state(2, 0));
    } else {
        expect_state_near(state, basis_state(2, 1));
    }
}

TEST(OperationsUnit, MeasureOnThreeQubits) {
    cqq::QStateVector state = {
        std::complex<double>(1.0 / std::sqrt(2.0), 0.0), // index 0 (|000>)
        std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0),
        std::complex<double>(1.0 / std::sqrt(2.0), 0.0), // index 4 (|100>)
        std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0)
    };
    unsigned creg;

    cqq::measure(state, 2, creg);

    EXPECT_TRUE(creg == 0 || creg == 1);
    if (creg == 0) {
        expect_state_near(state, basis_state(3, 0));
    } else {
        expect_state_near(state, basis_state(3, 4));
    }
}
