# PQC_Prsch / BadPK

> ⚠️ **Disclaimer**  
> This repository is an independent academic and personal study.  
> It is **not affiliated with, endorsed by, or developed in collaboration with**  
> Porsche AG or any other automotive manufacturer.  
> All telemetry scenarios and formats are used for **educational and research purposes only**.
---
## Overview

This repository provides a small experimental testbed to explore the use of
**Post-Quantum Cryptography (PQC) KEM mechanisms** for protecting
**automotive-like sensor telemetry**.

The main focus of the project is the analysis of **KEM failure behavior**
under **malformed or corrupted public key (Bad Public Key / BadPK) scenarios**.

Rather than optimizing for performance or production deployment,
this work aims to create a **minimal, reproducible, and educational environment**
to observe how different KEM designs react to invalid public keys.

---

## Project Structure

The project consists of two main modes:

1. **Mock mode**  
   A fully deterministic, educational KEM toy model designed to make
   key derivation and failure behavior explicit and easy to observe.

2. **Real mode**  
   Uses the standardized **HQC-128 (clean) implementation from PQClean**,
   representing a real post-quantum KEM with IND-CCA2 security.

The same BadPK experiment is applied to both modes.

---

## Bad Public Key (BadPK) Experiment

For each mode, the following comparison is performed:

- A shared secret derived using a **valid public key**
- A shared secret derived using a **single-bit corrupted public key (`pk_bad`)**

Expected behavior:

- **Valid public key** → shared secrets match
- **Corrupted public key** → shared secret mismatch or decapsulation failure

In the real HQC-128 implementation, the corrupted public key results in a
**decapsulation failure (`dec(badpk) fail`)**, as expected from a secure KEM.

---

## Build & Run

### 1. Mock (Educational) KEM

```bash
make mock
./badpk

EXAMPLE OUTPUT:
PK=32 SK=32 CT=16 SS=32
PK selfcheck = EQUAL
baseline match = YES
badpk match = NO

REAL HQC-128 KEM:
make clean
make real
./badpk

EXAMPLE OUTPUT:
PK=2249 SK=2305 CT=4433 SS=64
baseline match = YES
dec(badpk) fail //this confirms that HQC-128 correctly rejects malformed public keys during decapsulation.
