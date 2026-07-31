#include "cqq/states.h"

#include <complex>
#include <gtest/gtest.h>

template <typename Precision> class StatesTypedTest : public ::testing::Test {};
using StatesTestTypes = ::testing::Types<double, float>;
TYPED_TEST_SUITE(StatesTypedTest, StatesTestTypes);

TYPED_TEST(StatesTypedTest, NumQubitsReturnsZeroForOneAmplitudeState) {
    const cqq::QStateVector<> state = {std::complex<double>(1.0, 0.0)};
    EXPECT_EQ(cqq::num_qubits(state), 0u);
}

TYPED_TEST(StatesTypedTest, NumQubitsReturnsOneForTwoAmplitudes) {
    const cqq::QStateVector<> state = {
        std::complex<double>(1.0, 0.0), std::complex<double>(0.0, 0.0)};
    EXPECT_EQ(cqq::num_qubits(state), 1u);
}

TYPED_TEST(StatesTypedTest, NumQubitsReturnsThreeForEightAmplitudes) {
    const cqq::QStateVector<> state(8, std::complex<double>(0.0, 0.0));
    EXPECT_EQ(cqq::num_qubits(state), 3u);
}
