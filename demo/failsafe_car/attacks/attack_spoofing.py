import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Veriyi oku
df = pd.read_csv("data/porsche_telemetry.csv")
speed = df["speed_kmh"].values

# Saldırı aralığı
start = 2300
end   = 2600

# 1) Normal hız sinyali
normal_speed = speed.copy()

# 2) Spoofing – saldırganın sahte hızı
# Araç 120–140 km/h giderken saldırgan "55 km/h sabit gidiyor" desin
spoof_speed = speed.copy()
spoof_speed[start:end] = 55

# Normal sinyal spoofing sırasında gizlensin (görsellik için)
normal_masked = normal_speed.copy()
normal_masked[start:end] = np.nan

# Sadece spoof sinyalini göster
spoof_masked = np.full_like(speed, np.nan)
spoof_masked[start:end] = spoof_speed[start:end]

# Anomali işaretleri
anomaly_idx = np.arange(start, end, 20)

# Grafik
plt.figure(figsize=(14,6), facecolor="white")

# Gerçek tam sinyal (şeffaf mavi)
plt.plot(normal_speed, color="#cfe9ff", linewidth=1.0, label="Normal speed (full)")

# Spoofing penceresinde gerçek hız (gri referans)
plt.plot(range(start, end), speed[start:end],
         color="#777777", linewidth=1.8, linestyle="--",
         label="True speed in spoof window")

# Sahte hız (koyu kırmızı)
plt.plot(spoof_masked, color="#cc0000", linewidth=2.2,
         label="Spoofed (fake) speed")

# Anomali noktaları
plt.scatter(anomaly_idx, spoof_speed[anomaly_idx],
            color="orange", s=35, label="Detected anomalies")

plt.title("Spoofing Attack – Vehicle Speed Signal", fontsize=16)
plt.xlabel("Sample index")
plt.ylabel("Speed (km/h)")
plt.grid(True, alpha=0.3)
plt.legend(loc="upper right")

plt.tight_layout()
plt.savefig("data/spoof_attack_fixed.png", dpi=220)
print("Saved: data/spoof_attack_fixed.png")

