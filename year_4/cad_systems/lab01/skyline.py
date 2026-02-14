import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import make_lsq_spline

# Load image and convert to grayscale
img = cv2.imread("imgs/everest.jpg")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
height, width = gray.shape

# Edge detection
edges = cv2.Canny(gray, 50, 150)

plt.imshow(edges, cmap='gray')
plt.show()

# Extract skyline
x_coords = []
y_coords = []

for x in range(width):
    ys = np.where(edges[:, x])[0]
    if len(ys) > 0:
        y_coords.append(ys.min())
        x_coords.append(x)

x_coords = np.array(x_coords)
y_coords = np.array(y_coords)

# Flip y-axis
y_coords = height - 1 - y_coords

# Fit B-spline
degree = 2
points = 35

x_min, x_max = x_coords.min(), x_coords.max()
inner_knots = np.linspace(x_min, x_max, points - degree + 1)
knot_vector = np.concatenate((
    np.full(degree, x_min),
    inner_knots,
    np.full(degree, x_max)
))

spline = make_lsq_spline(x_coords, y_coords, knot_vector, k=degree)

# Evaluate spline
x_fine = np.linspace(x_min, x_max, 1000)
y_spline = spline(x_fine)

plt.figure(figsize=(12,6))
plt.imshow(np.flipud(gray), cmap='gray', origin="lower")
plt.plot(x_coords, y_coords, 'r.', markersize=2, label="Detected skyline")
plt.plot(x_fine, y_spline, 'b-', linewidth=2, label="B-spline fit")

for k in spline.t:
    plt.axvline(k, color='green', linestyle='--', alpha=0.5)

plt.legend()
plt.show()

print("knots =", list(map(lambda x: round(float(x)), spline.t)))
print("coeffs =", list(map(lambda x: round(float(x)), spline.c)))


