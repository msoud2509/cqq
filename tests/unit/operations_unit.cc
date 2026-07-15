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
