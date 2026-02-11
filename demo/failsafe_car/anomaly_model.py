import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.ensemble import IsolationForest

# === 1) VERİ ===
df = pd.read_csv("data/porsche_telemetry.csv")
speed = df["speed_kmh"].values

# === 1.1) Porsche stili smoothing (Hampel filter) ===
def hampel_filter(input_series, window_size=5, n_sigmas=3):
    new_series = input_series.copy()
    L = len(input_series)

    for i in range(window_size, L - window_size):
        window = input_series[i-window_size:i+window_size]
        median = np.median(window)
        std = np.std(window)

        if abs(input_series[i] - median) > n_sigmas * std:
            new_series[i] = median  # düzelt

    return new_series

speed_smooth = hampel_filter(speed, window_size=8)

# === 2) MODEL ===
model = IsolationForest(contamination=0.01, random_state=42)
model.fit(speed_smooth.reshape(-1,1))

pred = model.predict(speed_smooth.reshape(-1,1))
model_anomaly_idx = np.where(pred == -1)[0]
real_anomaly_idx = np.where(df["is_anomaly"] == 1)[0]

# === 3) ERROR (normalize edilmiş) ===
raw_error = (speed_smooth - np.mean(speed_smooth))**2
error_norm = raw_error / np.max(raw_error)

# === 4) GRAFİK — Porsche Style ===
plt.figure(figsize=(16, 7))

plt.plot(speed_smooth, label="signal",
         color="#6A5ACD", linewidth=1.5, alpha=0.9)

plt.fill_between(range(len(error_norm)),
                 error_norm * 150, color="#FF6F6F",
                 alpha=0.25, label="prediction quadratic error")

plt.scatter(model_anomaly_idx, speed_smooth[model_anomaly_idx],
            color="#FF8C00", s=50, label="Model anomaly")

plt.scatter(real_anomaly_idx, speed_smooth[real_anomaly_idx],
            color="#00CED1", s=60, label="Real anomaly")

plt.title("Porsche Telemetry Anomaly Detection", fontsize=16)
plt.xlabel("Time Index")
plt.ylabel("Speed / Error")
plt.grid(True, alpha=0.3)
plt.legend()

plt.savefig("data/anomaly_plot_porsche_style.png", dpi=300)
print("Saved: data/anomaly_plot_porsche_style.png")

plt.show()

