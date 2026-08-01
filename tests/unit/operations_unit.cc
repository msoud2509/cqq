#include "cqq/operations.h"

#include <cmath>
#include <complex>

#include <gtest/gtest.h>

namespace {

cqq::QStateVector<double> basis_state(size_t num_qubits, size_t basis_index) {
    cqq::QStateVector<> state(1ULL << num_qubits, std::complex<double>(0.0, 0.0));
    state[basis_index] = std::complex<double>(1.0, 0.0);
    return state;
}

void expect_state_near(const cqq::QStateVector<double>& actual,
    const cqq::QStateVector<double>& expected, double tolerance = 1e-12) {
    ASSERT_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < actual.size(); ++i) {
        EXPECT_NEAR(actual[i].real(), expected[i].real(), tolerance) << "at index " << i;
        EXPECT_NEAR(actual[i].imag(), expected[i].imag(), tolerance) << "at index " << i;
    }
}

} // namespace

template <typename Precision> class OperationsTypedTest : public ::testing::Test {};
using OperationsTestTypes = ::testing::Types<double, float>;
TYPED_TEST_SUITE(OperationsTypedTest, OperationsTestTypes);

////////////////////////////////
// Hadamard Gate Tests
////////////////////////////////
TYPED_TEST(OperationsTypedTest, HadamardMapsZeroToPlusState) {
    auto state = basis_state(1, 0);

    cqq::apply_hadamard(state, 0);

    const auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    const cqq::QStateVector<> expected = {
        std::complex<double>(inv_sqrt2, 0.0), std::complex<double>(inv_sqrt2, 0.0)};

    expect_state_near(state, expected);
}

TYPED_TEST(OperationsTypedTest, HadamardIsItsOwnInverse) {
    auto state = basis_state(1, 0);

    cqq::apply_hadamard(state, 0);
    cqq::apply_hadamard(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

TYPED_TEST(OperationsTypedTest, HadamardOnTwoQubits) {
    auto state = basis_state(2, 0);

    cqq::apply_hadamard(state, 0);
    cqq::apply_hadamard(state, 1);

    const auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    const cqq::QStateVector<> expected = {std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0),
        std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0),
        std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0),
        std::complex<double>(inv_sqrt2 * inv_sqrt2, 0.0)};

    expect_state_near(state, expected);
}

TYPED_TEST(OperationsTypedTest, HadamardOnOneQubitInTwoQubitSystem) {
    auto state = basis_state(2, 0);

    cqq::apply_hadamard(state, 1);

    const auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    const cqq::QStateVector<> expected = {std::complex<double>(inv_sqrt2, 0.0),
        std::complex<double>(0.0, 0.0), std::complex<double>(inv_sqrt2, 0.0),
        std::complex<double>(0.0, 0.0)};

    expect_state_near(state, expected);
}

////////////////////////////////
// Pauli-X Gate Tests
////////////////////////////////
TYPED_TEST(OperationsTypedTest, PauliXMapsZeroToOne) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_x(state, 0);

    expect_state_near(state, basis_state(1, 1));
}

TYPED_TEST(OperationsTypedTest, PauliXIsItsOwnInverse) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_x(state, 0);
    cqq::apply_pauli_x(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

TYPED_TEST(OperationsTypedTest, PauliXOnTwoQubits) {
    auto state = basis_state(2, 0);

    cqq::apply_pauli_x(state, 0);
    cqq::apply_pauli_x(state, 1);

    expect_state_near(state, basis_state(2, 3));
}

////////////////////////////////
// Pauli-Z Gate Tests
////////////////////////////////
TYPED_TEST(OperationsTypedTest, PauliZMapsOneToNegativeOne) {
    auto state = basis_state(1, 1);

    cqq::apply_pauli_z(state, 0);

    const cqq::QStateVector<> expected = {
        std::complex<double>(0.0, 0.0), std::complex<double>(-1.0, 0.0)};
    expect_state_near(state, expected);
}

TYPED_TEST(OperationsTypedTest, PauliZDoesNotAffectZero) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_z(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

TYPED_TEST(OperationsTypedTest, PauliZIsItsOwnInverse) {
    auto state = basis_state(1, 1);

    cqq::apply_pauli_z(state, 0);
    cqq::apply_pauli_z(state, 0);

    expect_state_near(state, basis_state(1, 1));
}

TYPED_TEST(OperationsTypedTest, PauliZOnTwoQubits) {
    auto state = basis_state(2, 3);

    cqq::apply_pauli_z(state, 0);
    cqq::apply_pauli_z(state, 1);

    const cqq::QStateVector<> expected = {std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
        std::complex<double>(1.0, 0.0)};
    expect_state_near(state, expected);
}

////////////////////////////////
// Pauli-Y Gate Tests
////////////////////////////////
TYPED_TEST(OperationsTypedTest, PauliYMapsZeroToI) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_y(state, 0);

    const cqq::QStateVector<> expected = {
        std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 1.0)};
    expect_state_near(state, expected);
}

TYPED_TEST(OperationsTypedTest, PauliYMapsOneToNegativeI) {
    auto state = basis_state(1, 1);

    cqq::apply_pauli_y(state, 0);

    const cqq::QStateVector<> expected = {
        std::complex<double>(0.0, -1.0), std::complex<double>(0.0, 0.0)};
    expect_state_near(state, expected);
}

TYPED_TEST(OperationsTypedTest, PauliYIsItsOwnInverse) {
    auto state = basis_state(1, 0);

    cqq::apply_pauli_y(state, 0);
    cqq::apply_pauli_y(state, 0);

    expect_state_near(state, basis_state(1, 0));
}

////////////////////////////////
// SWAP Gate Tests
////////////////////////////////
TYPED_TEST(OperationsTypedTest, SwapSwapsTwoQubits) {
    auto state = basis_state(2, 1); // origingally set to |01>

    cqq::apply_swap(state, 0, 1);

    expect_state_near(state, basis_state(2, 2)); // |10>
}

TYPED_TEST(OperationsTypedTest, SwapIsItsOwnInverse) {
    auto state = basis_state(2, 1); // |01>

    cqq::apply_swap(state, 0, 1);
    cqq::apply_swap(state, 0, 1);

    expect_state_near(state, basis_state(2, 1)); // |01>
}

TYPED_TEST(OperationsTypedTest, SwapOnTwoQubits) {
    auto state = basis_state(2, 0); // |00>

    cqq::apply_swap(state, 0, 1);

    expect_state_near(state, basis_state(2, 0)); // |00>
}

TYPED_TEST(OperationsTypedTest, SwapOnThreeQubits) {
    auto state = basis_state(3, 1); // |001>

    cqq::apply_swap(state, 0, 2);

    expect_state_near(state, basis_state(3, 4)); // |100>
}

////////////////////////////////
// CNOT Gate Tests
////////////////////////////////
TYPED_TEST(OperationsTypedTest, CNOTFlipsTargetWhenControlIsOne) {
    auto state = basis_state(2, 3); // |11>

    cqq::apply_controlled_not(state, 0, 1);

    expect_state_near(state, basis_state(2, 1)); // |10>
}

TYPED_TEST(OperationsTypedTest, CNOTDoesNotFlipTargetWhenControlIsZero) {
    auto state = basis_state(2, 1); // |01>

    cqq::apply_controlled_not(state, 1, 0);

    expect_state_near(state, basis_state(2, 1)); // |01>
}

TYPED_TEST(OperationsTypedTest, CNOTIsItsOwnInverse) {
    auto state = basis_state(2, 3); // |11>

    cqq::apply_controlled_not(state, 0, 1);
    cqq::apply_controlled_not(state, 0, 1);

    expect_state_near(state, basis_state(2, 3)); // |11>
}

////////////////////////////////
// Measurement Tests
////////////////////////////////
TYPED_TEST(OperationsTypedTest, MeasureCollapsesStateToBasisState) {
    auto state = basis_state(1, 0);
    unsigned creg;

    cqq::measure(state, 0, creg);

    EXPECT_EQ(creg, 0);
    expect_state_near(state, basis_state(1, 0));
}

TYPED_TEST(OperationsTypedTest, MeasureCollapsesSuperpositionToBasisState) {
    cqq::QStateVector<> state = {std::complex<double>(1.0 / std::sqrt(2.0), 0.0),
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

TYPED_TEST(OperationsTypedTest, MeasureThrowsOnInvalidQubitIndex) {
    auto state = basis_state(1, 0);
    unsigned creg;

    EXPECT_THROW(cqq::measure(state, 1, creg), std::invalid_argument);
}

TYPED_TEST(OperationsTypedTest, MeasureOnTwoQubits) {
    cqq::QStateVector<> state = {std::complex<double>(1.0 / std::sqrt(2.0), 0.0),
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

TYPED_TEST(OperationsTypedTest, MeasureOnThreeQubits) {
    cqq::QStateVector<> state = {std::complex<double>(1.0 / std::sqrt(2.0), 0.0), // index 0 (|000>)
        std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0),
        std::complex<double>(1.0 / std::sqrt(2.0), 0.0), // index 4 (|100>)
        std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0),
        std::complex<double>(0.0, 0.0)};
    unsigned creg;

    cqq::measure(state, 2, creg);

    EXPECT_TRUE(creg == 0 || creg == 1);
    if (creg == 0) {
        expect_state_near(state, basis_state(3, 0));
    } else {
        expect_state_near(state, basis_state(3, 4));
    }
}
