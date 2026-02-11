import numpy as np
import pandas as pd
from datetime import datetime, timedelta

def generate_porsche_telemetry(num_samples=5000):

    start_time = datetime(2025, 12, 4, 10, 0, 0)
    timestamps = [start_time + timedelta(milliseconds=50*i) for i in range(num_samples)]

    #Porsche telemetry realistic ranges 
    speed = np.clip(np.random.normal(120, 15, num_samples), 0, 280)
    rpm = np.clip(np.random.normal(4500, 800, num_samples), 800, 9000)
    throttle = np.clip(np.random.normal(40, 20, num_samples), 0, 100)
    brake = np.clip(np.random.normal(5, 10, num_samples), 0, 100)
    steering = np.random.normal(0, 12, num_samples)
    gear = np.random.choice([1,2,3,4,5,6,7,8], num_samples)
    engine_temp = np.clip(np.random.normal(90, 8, num_samples), 70, 120)
    battery_voltage = np.clip(np.random.normal(13.2, 0.3, num_samples), 11.5, 14.8)
    acc_long = np.random.normal(0.05, 0.15, num_samples)
    acc_lat = np.random.normal(0.02, 0.12, num_samples)
    
    anomaly_indices = np.random.choice(range(200, num_samples-200), 5)
    for idx in anomaly_indices:
        rpm[idx] += 3000
        brake[idx] = 0
        throttle[idx] = 100
        acc_long[idx] += 1.8
        acc_lat[idx] += 1.2
        speed[idx] -= 40

    df = pd.DataFrame({
        "timestamp": timestamps,
        "speed_kmh": speed,
        "rpm": rpm,
        "throttle_pos": throttle,
        "brake_pressure": brake,
        "steering_angle": steering,
        "gear": gear,
        "engine_temp": engine_temp,
        "battery_voltage": battery_voltage,
        "longitudinal_acc": acc_long,
        "lateral_acc": acc_lat,
        "is_anomaly": [1 if i in anomaly_indices else 0 for i in range(num_samples)]
    })

    return df


if __name__ == "__main__":
    df = generate_porsche_telemetry()
    df.to_csv("data/porsche_telemetry.csv", index=False)
    print("Generated: data/porsche_telemetry.csv")

