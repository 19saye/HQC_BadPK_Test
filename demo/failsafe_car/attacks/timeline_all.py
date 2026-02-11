k"""
attacks/timeline_all.py
Replay + FDIA + Spoofing unified timeline (telemetri zamanında).
"""

import pandas as pd
import matplotlib.pyplot as plt

from .event_engine import EventEngine
from .secure_layer import SecureLayer


def run_all() -> None:
    df = pd.read_csv("data/porsche_telemetry.csv")
    ts = pd.to_datetime(df["timestamp"])
    t0 = ts.iloc[0]
    tsec = (ts - t0).dt.total_seconds().values
    speed = df["speed_kmh"].values

    evt = EventEngine()
    sec = SecureLayer(evt)
    expected_key = "ABCD1234"

    # Replay
    for i in range(1000, 1300):
        sec.process_packet(
            attack_kind="replay",
            sample_idx=i,
            logical_time=float(tsec[i]),
            original_value=float(speed[i]),
            incoming_value=float(speed[i - 300]),
            expected_key=expected_key,
            received_key=expected_key,
            note="replayed_old_segment",
        )

    # FDIA
    for i in range(1600, 1900):
        sec.process_packet(
            attack_kind="fdia",
            sample_idx=i,
            logical_time=float(tsec[i]),
            original_value=float(speed[i]),
            incoming_value=float(speed[i] * 0.60 + 15.0),
            expected_key=expected_key,
            received_key=expected_key,
            note="bias_and_scale",
        )

    # Spoofing
    for i in range(2300, 2600):
        sec.process_packet(
            attack_kind="spoofing",
            sample_idx=i,
            logical_time=float(tsec[i]),
            original_value=float(speed[i]),
            incoming_value=55.0,
            expected_key=expected_key,
            received_key=expected_key,
            note="fixed_fake_speed",
        )

    # Plot
    plt.figure(figsize=(16, 7), facecolor="white")

    def plot_kind(kind: str, label: str):
        xs = [e.logical_time for e in evt.events if e.kind == kind]
        ys = [e.sample_idx for e in evt.events if e.kind == kind]
        plt.scatter(xs, ys, s=14, label=label)

    plot_kind("replay", "Replay attack")
    plot_kind("fdia", "FDIA attack")
    plot_kind("spoofing", "Spoofing attack")
    plot_kind("failsafe", "Fail-safe (key mismatch)")

    plt.title("Unified PQC Fail-Safe Timeline (Telemetry Time)", fontsize=16)
    plt.xlabel("Telemetry time since start (s)")
    plt.ylabel("Sample index (i)")
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.tight_layout()
    plt.savefig("data/timeline_ALL_attacks.png", dpi=220)
    print("Saved: data/timeline_ALL_attacks.png")


if __name__ == "__main__":
    run_all()

