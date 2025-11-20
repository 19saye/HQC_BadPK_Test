#!/usr/bin/env python3
import re
import sys
from pathlib import Path

def parse_file(path: Path):
    text = path.read_text(encoding="utf-8", errors="ignore")

    m_sizes = re.search(r"PK=(\d+)\s+SK=(\d+)\s+CT=(\d+)\s+SS=(\d+)", text)
    if not m_sizes:
        return None

    pk, sk, ct, ss = map(int, m_sizes.groups())

    m_base = re.search(r"baseline match = (\w+)", text)
    baseline = m_base.group(1) if m_base else "?"

    badpk_match = "?"  # default
    m_bad = re.search(r"badpk match = (\w+)", text)
    if m_bad:
        badpk_match = m_bad.group(1)
    elif "dec(badpk) fail" in text:
        badpk_match = "FAIL(dec)"

    name = path.name
    if "mock" in name:
        scheme = "mock"
    elif "real" in name:
        scheme = "HQC-128"
    else:
        scheme = "unknown"

    return {
        "file": name,
        "scheme": scheme,
        "PK": pk,
        "SK": sk,
        "CT": ct,
        "SS": ss,
        "baseline": baseline,
        "badpk": badpk_match,
    }

def main():
    if len(sys.argv) > 1:
        files = [Path(p) for p in sys.argv[1:]]
    else:
        # varsayılan: run_*.txt’leri tara
        files = sorted(Path(".").glob("run_*.txt"))

    rows = [parse_file(p) for p in files]
    rows = [r for r in rows if r]

    if not rows:
        print("No logs found.")
        return

    header = ["file", "scheme", "PK", "SK", "CT", "SS", "baseline", "badpk"]
    print(" | ".join(header))
    print("-" * 80)
    for r in rows:
        print(" | ".join(str(r[k]) for k in header))

if __name__ == "__main__":
    main()

