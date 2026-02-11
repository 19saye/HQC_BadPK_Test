"""
attacks/timeline_fdia.py
FDIA senaryosu için PQC + fail-safe timeline (telemetri zamanında).
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from .event_engine import EventEngine
from .secure_layer import SecureLayer


def run_fdia_scenario() -> None:
    df = pd.read_csv("data/porsche_telemetry.csv")
    ts = pd.to_datetime(df["timestamp"])
    t0 = ts.iloc[0]
    tsec = (ts - t0).dt.total_seconds().values

    speed = df["speed_kmh"].values

    evt = EventEngine()
    sec = SecureLayer(evt)

    expected_key = "ABCD1234"

    start = 1600
    end   = 1900

    for i in range(start, end):
        original = float(speed[i])

        # FDIA: ölçümü boz (demo)
        injected = float(speed[i] * 0.60 + 15.0)

        sec.process_packet(
            attack_kind="fdia",
            sample_idx=i,
            logical_time=float(tsec[i]),
            original_value=original,
            incoming_value=injected,
            expected_key=expected_key,
            received_key=expected_key,
            note="bias_and_scale",
        )

    times = [e.logical_time for e in evt.events]
    y     = [e.sample_idx for e in evt.events]

    plt.figure(figsize=(14, 6), facecolor="white")
    plt.scatter(times, y, s=16, label="FDIA events")

    plt.title("FDIA Attack – PQC Fail-Safe Timeline (Telemetry Time)", fontsize=16)
    plt.xlabel("Telemetry time since start (s)")
    plt.ylabel("Sample index (i)")
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.tight_layout()
    plt.savefig("data/timeline_fdia.png", dpi=220)
    print("Saved: data/timeline_fdia.png")


if __name__ == "__main__":
    run_fdia_scenario()

