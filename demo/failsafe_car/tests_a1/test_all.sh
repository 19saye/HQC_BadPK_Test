#!/bin/bash

echo " RUNNING ALL TESTS "

echo -n "[A1] Keypair Test ................ "
./test_keypair >/dev/null && echo "PASS" || echo "FAIL"

echo -n "[A2] Cipher Test ................. "
./test_cipher >/dev/null && echo "PASS" || echo "FAIL"

echo -n "[A3] CRC Test .................... "
./test_crc >/dev/null && echo "PASS" || echo "FAIL"

echo -n "[A4] Fail-safe Injection Test .... "
./test_failsafe >/dev/null && echo "PASS" || echo "FAIL"

echo "TEST SUITE COMPLETE "
echo " RUNNING ALL TESTS "

# A1
./test_keypair && echo "[A1] Keypair Test ................ PASS" || echo "[A1] Keypair Test ................ FAIL"

# A2
./test_cipher && echo "[A2] Cipher Test ................. PASS" || echo "[A2] Cipher Test ................. FAIL"

# A3
./test_crc && echo "[A3] CRC Test .................... PASS" || echo "[A3] CRC Test .................... FAIL"

# A4
./test_failsafe && echo "[A4] Fail-safe Injection Test .... PASS" || echo "[A4] Fail-safe Injection Test .... FAIL"

echo "TEST SUITE COMPLETE"

