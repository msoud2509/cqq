OPENQASM 2.0;
include "qelib1.inc";

qreg q[10];
creg c[10];

// Sprinkle some distinct states into the vector
x q[0];
y q[2];
z q[4];
x q[6];
y q[8];

h q[1];
h q[3];
h q[5];
h q[7];
h q[9];

// Reverse the order of the entire 10-qubit register
swap q[0], q[9];
swap q[1], q[8];
swap q[2], q[7];
swap q[3], q[6];
swap q[4], q[5];

// Measure all qubits
measure q[0] -> c[0];
measure q[1] -> c[1];
measure q[2] -> c[2];
measure q[3] -> c[3];
measure q[4] -> c[4];
measure q[5] -> c[5];
measure q[6] -> c[6];
measure q[7] -> c[7];
measure q[8] -> c[8];
measure q[9] -> c[9];
