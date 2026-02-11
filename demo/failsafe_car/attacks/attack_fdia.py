# attacks/attack_fdia.py
# Rabi'nin FDIA (False Data Injection Attack) görselleştirmesi

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# 1) Telemetri verisini yükle
df = pd.read_csv("data/porsche_telemetry.csv")

speed = df["speed_kmh"].values  # Normal hız sinyali

# 2) FDIA uygulanacak aralık (örnek: 2200–2600 arası)
start = 2200
end   = 2600

# Orijinal sinyalin kopyaları
normal_speed = speed.copy()
fdia_speed   = speed.copy()

# 3) False Data Injection:
# Bu blokta gerçek hızı sanki araç "çok daha yavaş gidiyormuş" gibi gösterelim
# (örneğin %35 düşürelim + küçük bir bias ekleyelim)
fdia_speed[start:end] = fdia_speed[start:end] * 0.65 + 5.0

# Plot’ta daha net görmek için:
normal_masked = normal_speed.copy()
normal_masked[start:end] = np.nan     # FDIA bölgesinde normal gizlensin

fdia_masked = np.full_like(fdia_speed, np.nan)
fdia_masked[start:end] = fdia_speed[start:end]  # Sadece FDIA bölgesi gösterilsin

# 4) Basit anomaly işaretleri (sadece FDIA bloğu içinde)
anomaly_idx = np.arange(start, end, 15)

# === GRAFİK ===
plt.figure(figsize=(14, 6), facecolor="white")

# Normal sürüş
plt.plot(normal_speed, color="#cfe9ff", linewidth=1.0, label="Normal speed (full)")

# FDIA uygulanan aralıkta gerçek normal hız (gri ile referans)
plt.plot(range(start, end), speed[start:end],
         color="#999999", linewidth=1.8, linestyle="--",
         label="True speed in FDIA window")

# FDIA ile manipüle edilen hız (kırmızı)
plt.plot(fdia_masked, color="#d62728", linewidth=2.2,
         label="FDIA–corrupted speed")

# Modelin tespit ettiği “şüpheli noktalar” (turuncu)
plt.scatter(anomaly_idx, fdia_speed[anomaly_idx],
            color="orange", s=35, label="Injected anomalies")

plt.title("False Data Injection (FDIA) – Vehicle Speed Signal", fontsize=16)
plt.xlabel("Sample index")
plt.ylabel("Speed (km/h)")
plt.grid(True, alpha=0.3)
plt.legend(loc="upper right")

plt.tight_layout()
plt.savefig("data/fdia_attack_fixed.png", dpi=220)
print("Saved: data/fdia_attack_fixed.png")

