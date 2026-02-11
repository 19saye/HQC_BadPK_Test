"""
attacks/timeline_spoofing.py
Spoofing senaryosu için PQC + fail-safe timeline (telemetri zamanında).
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from .event_engine import EventEngine
from .secure_layer import SecureLayer


def run_spoofing_scenario() -> None:
    df = pd.read_csv("data/porsche_telemetry.csv")
    ts = pd.to_datetime(df["timestamp"])
    t0 = ts.iloc[0]
    tsec = (ts - t0).dt.total_seconds().values

    speed = df["speed_kmh"].values

    evt = EventEngine()
    sec = SecureLayer(evt)

    expected_key = "ABCD1234"

    start = 2300
    end   = 2600

    for i in range(start, end):
        original = float(speed[i])

        # Spoofing: sabit sahte değer ver (demo)
        spoofed = 55.0

        sec.process_packet(
            attack_kind="spoofing",
            sample_idx=i,
            logical_time=float(tsec[i]),
            original_value=original,
            incoming_value=spoofed,
            expected_key=expected_key,
            received_key=expected_key,
            note="fixed_fake_speed",
        )

    times = [e.logical_time for e in evt.events]
    y     = [e.sample_idx for e in evt.events]

    plt.figure(figsize=(14, 6), facecolor="white")
    plt.scatter(times, y, s=16, label="Spoofing events")

    plt.title("Spoofing Attack – PQC Fail-Safe Timeline (Telemetry Time)", fontsize=16)
    plt.xlabel("Telemetry time since start (s)")
    plt.ylabel("Sample index (i)")
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.tight_layout()
    plt.savefig("data/timeline_spoofing.png", dpi=220)
    print("Saved: data/timeline_spoofing.png")


if __name__ == "__main__":
    run_spoofing_scenario()

