import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage.feature import match_template
from matplotlib.patches import Rectangle


def show(img_rgb, title=None):
    fig = plt.figure(figsize=(6, 6))
    ax = fig.add_subplot(111)
    ax.imshow(img_rgb, cmap="gray")
    ax.axis("off")
    if title:
        ax.set_title(title)
    plt.show()


def extract_template(img, size):
    h, w = img.shape
    cy, cx = h // 2, w // 2
    y, x = cy - size // 2, cx - size // 2
    return img[y : y + size, x : x + size]


def distort_image(img, noise_level=25):
    noisy = img.astype(np.float32)
    noisy += np.random.normal(0, noise_level, img.shape)
    noisy = np.clip(noisy, 0, 255)

    return noisy.astype(np.uint8)

import numpy as np

def convolution(image, mask):
    img_h, img_w = image.shape
    h, w = mask.shape

    pad_h, pad_w = h // 2, w // 2

    out_h = img_h - 2 * pad_h
    out_w = img_w - 2 * pad_w
    out = np.zeros((out_h, out_w), dtype=float)

    for i in range(out_h):
        for j in range(out_w):
            region = image[i : i + h, j : j + w]
            out[i, j] = np.sum(region * mask)

    return np.abs(out).astype(np.uint8)



if __name__ == "__main__":
    img_path = "./data/lena.png"
    img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)

    show(img, "Obraz wejściowy")

    template = distort_image(extract_template(img, 64))
    show(template, "Szablon (template) wybrany z obrazu")

    corr_map = match_template(img, template)
    y_max, x_max = np.unravel_index(np.argmax(corr_map), corr_map.shape)

    fig = plt.figure(figsize=(8, 8))
    ax = fig.add_subplot(111)
    ax.imshow(img, cmap="gray")
    rect = Rectangle(
        (x_max, y_max), template.shape[1], template.shape[0], linewidth=3, fill=False
    )
    ax.add_patch(rect)
    ax.axis("off")
    ax.set_title(
        "Wynik dopasowania szablonu"
    )
    plt.show()

    mask = np.array([
        [-1, 0, 1], 
        [-1, 0, 1], 
        [-1, 0, 1]
    ])
    conv_img = convolution(img, mask)
    show(conv_img, "Wynik konwolucji")
