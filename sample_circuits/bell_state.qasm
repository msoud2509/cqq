OPENQASM 2.0;
include "qelib1.inc";

qreg q[2];
creg c[2];

// Create superposition on q0
h q[0];

// Entangle q0 and q1
cx q[0], q[1];

measure q[0] -> c[0];
measure q[1] -> c[1];
