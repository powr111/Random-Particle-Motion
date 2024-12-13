import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os  # To handle file paths

# Initialize lists for storing data
times = []
diffusion_constants = []
x_means = []

# Loop through files and extract required data
for i in range(9999):
    file_path = os.path.join("output", "Data", f"particle_motion_{i}.csv")
    data = pd.read_csv(file_path)

    # Ensure column names match those in the CSV file
    times.append(data["time"].max())
    diffusion_constants.append(data["Diffusion_Constant"].mean() if "Diffusion_Constant" in data else np.nan)
    x_means.append(data["x_coordinates"].mean())

# Convert lists to NumPy arrays for plotting
times = np.array(times)
diffusion_constants = np.array(diffusion_constants)
x_means = np.array(x_means) ** 2  # Square the X values

# Create subplots for side-by-side scatter plots
fig, axes = plt.subplots(1, 2, figsize=(12, 6))

# Plot Diffusion Constant vs. X-Coordinate
axes[0].scatter(diffusion_constants, x_means, color="blue", label="Diffusion Constant vs. X-Coordinate")
axes[0].set_ylabel("X Coordinate (Mean Squared)")
axes[0].set_xlabel("Diffusion Constant")
axes[0].set_title("Diffusion Constant vs. X-Coordinate")
axes[0].legend()

# Plot Time vs. X-Coordinate
axes[1].scatter(times, x_means, color="green", label="Time vs. X-Coordinate")
axes[1].set_ylabel("X Coordinate (Mean Squared)")
axes[1].set_xlabel("Time")
axes[1].set_title("Time vs. X-Coordinate")
axes[1].legend()

# Adjust layout and show the plots
plt.tight_layout()
plt.show()
