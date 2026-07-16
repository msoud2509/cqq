#include "cqq/states.h"

#include <complex>

#include <gtest/gtest.h>

TEST(StatesUnit, NumQubitsReturnsZeroForOneAmplitudeState) {
    const cqq::QStateVector state = {std::complex<double>(1.0, 0.0)};
    EXPECT_EQ(cqq::num_qubits(state), 0u);
}

TEST(StatesUnit, NumQubitsReturnsOneForTwoAmplitudes) {
    const cqq::QStateVector state = {
        std::complex<double>(1.0, 0.0), std::complex<double>(0.0, 0.0)};
    EXPECT_EQ(cqq::num_qubits(state), 1u);
}

TEST(StatesUnit, NumQubitsReturnsThreeForEightAmplitudes) {
    const cqq::QStateVector state(8, std::complex<double>(0.0, 0.0));
    EXPECT_EQ(cqq::num_qubits(state), 3u);
}
