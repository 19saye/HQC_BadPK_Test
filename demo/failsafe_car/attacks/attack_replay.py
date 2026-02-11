import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 1) Telemetri verisini oku (failsafe_car içinden çalıştırıyoruz)
df = pd.read_csv("data/porsche_telemetry.csv")
speed = df["speed_kmh"].values

# 2) Replay attack aralığı (örnek: 1800–2100)
start = 1800
end = 2100

normal = speed.copy()
attack = speed.copy()

# Normal sinyali göster ama replay aralığını gizle
normal[start:end] = float("nan")

# Replay segmentini vurgula: sadece o aralık kalsın
attack[:start] = float("nan")
attack[end:] = float("nan")

# 3) Grafik
plt.figure(figsize=(14, 6), facecolor="white")

plt.plot(normal, color="lightblue", alpha=0.6, label="Normal speed")
plt.plot(attack, color="#0033cc", linewidth=1.8, label="Replayed segment")

# Anomali noktalarını işaretle
anomaly_idx = np.arange(start + 20, start + 140, 7)
plt.scatter(anomaly_idx, attack[anomaly_idx],
            color="orange", s=30, label="Detected anomalies")

plt.title("Replay Attack – Vehicle Speed Signal", fontsize=16)
plt.xlabel("Sample index")
plt.ylabel("Speed (km/h)")
plt.grid(True, alpha=0.3)
plt.legend()

plt.tight_layout()
plt.savefig("data/replay_attack_fixed.png", dpi=220)
print("Saved: data/replay_attack_fixed.png")

