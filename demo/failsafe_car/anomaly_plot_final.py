import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# === LOAD DATA ===
df = pd.read_csv("data/porsche_telemetry.csv")

speed = df["speed_kmh"].values
rpm = df["rpm"].values
throttle = df["throttle_pos"].values
brake = df["brake_pressure"].values
real_anom = df["is_anomaly"].values

# --- MODEL BASED ANOMALY (simple reconstruction error) ---
from sklearn.ensemble import IsolationForest
X = np.column_stack([speed, rpm, throttle, brake])

model = IsolationForest(contamination=0.01, random_state=42)
model.fit(X)
scores = -model.score_samples(X)

threshold = np.percentile(scores, 98)
model_anom = scores > threshold

# === PLOTS ===

plt.style.use("seaborn-v0_8-darkgrid")
fig, ax = plt.subplots(2, 1, figsize=(16, 10), sharex=True)

# --- Panel 1: Speed ---
ax[0].plot(speed, color="#5148f0", linewidth=1.0, label="Vehicle speed (km/h)")
ax[0].scatter(np.where(model_anom)[0], speed[model_anom],
              color="#ff7f00", s=30, label="Model anomaly")

ax[0].scatter(np.where(real_anom==1)[0], speed[real_anom==1],
              color="#00dbff", s=50, label="Real anomaly")

ax[0].set_ylabel("Speed (km/h)")
ax[0].set_title("Porsche Telemetry – Multi-signal Anomaly Detection")
ax[0].legend(loc="upper right")

# --- Panel 2: Error ---
ax[1].plot(scores, color="#fb6464", alpha=0.7, label="Model error")
ax[1].axhline(threshold, color="black", linestyle="--", linewidth=1, label="Error threshold")

ax[1].scatter(np.where(model_anom)[0], scores[model_anom],
              color="#ff7f00", s=25)

ax[1].set_ylabel("Reconstruction Error")
ax[1].set_xlabel("Time index")
ax[1].legend(loc="upper right")

plt.tight_layout()
plt.savefig("data/porsche_telemetry_anomaly_final.png", dpi=300)
plt.show()

