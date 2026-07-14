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
