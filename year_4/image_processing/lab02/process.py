import cv2
import matplotlib.pyplot as plt
import numpy as np


def add_noise(img, noise_level=75):
    noisy = img.astype(np.float32)
    noisy += np.random.normal(0, noise_level, img.shape)
    noisy = np.clip(noisy, 0, 255)
    return noisy.astype(np.uint8)


def show2(img1, img2, title1="", title2=""):
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 3, 1)
    plt.title(title1)
    plt.imshow(img1, cmap="gray")
    plt.axis("off")

    plt.subplot(1, 3, 2)
    plt.title(title2)
    plt.imshow(img2, cmap="gray")
    plt.axis("off")

    plt.subplot(1, 3, 3)
    plt.title("Difference")
    plt.imshow(cv2.absdiff(img1, img2), cmap="gray")
    plt.axis("off")

    plt.tight_layout()
    plt.show()

def quantize_image(img, levels=64):
    img = img * 256
    step = 256 // levels
    quantized = (img // step) * step
    quantized = np.clip(quantized, 0, 255)
    return quantized.astype(np.uint8)

def transform(img, keep=0.1):
    img = np.float32(img) / 255.0
    dct = cv2.dct(img)

    h, w = img.shape
    mask = np.zeros((h, w), np.uint8)
    h_keep = int(h * keep)
    w_keep = int(w * keep)

    mask[:h_keep, :w_keep] = 1
    dct_masked = dct * mask
    plt.imshow(dct_masked, cmap="gray")
    plt.show()
    img_reconstructed = cv2.idct(dct_masked)

    return img_reconstructed


if __name__ == "__main__":
    img = cv2.imread("data/lena2.png", cv2.IMREAD_GRAYSCALE)
    img = cv2.resize(img, (1024, 1024))

    img_noisy = add_noise(img)
    keep = 0.1

    reconstructed = quantize_image(transform(img, keep), 128)
    reconstructed_noisy = quantize_image(transform(img_noisy, keep), 128)

    img = img.astype(np.uint8)
    img_noisy = img_noisy.astype(np.uint8)

    show2(
        img,
        reconstructed,
        "Original",
        f"Reconstructed",
    )

    show2(
        img_noisy,
        reconstructed_noisy,
        "Noisy",
        f"Reconstructed Noisy",
    )
