import cv2
import matplotlib.pyplot as plt
import numpy as np
import pywt
from sklearn.metrics.pairwise import cosine_similarity


def create_emoticon(emotion):
    img = np.zeros((64, 64), dtype=np.uint8)

    # Face
    cv2.circle(img, (32, 32), 30, 255, 2)

    # Eyes
    cv2.circle(img, (22, 24), 3, 255, -1)
    cv2.circle(img, (42, 24), 3, 255, -1)

    # Mouth
    if emotion == "smile":
        cv2.ellipse(img, (32, 40), (12, 5), 0, 0, 180, 255, 2)
    elif emotion == "sad":
        cv2.ellipse(img, (32, 40), (12, 5), 0, 180, 360, 255, 2)
    elif emotion == "neutral":
        cv2.line(img, (22, 42), (42, 42), 255, 2)
    elif emotion == "surprise":
        cv2.circle(img, (32, 42), 5, 255, 2)
    else:
        raise ValueError("Unknown emotion: {}".format(emotion))
    return img


def distort_image(img, noise_level=25, deform_shift=8):
    noisy = img.astype(np.float32)
    noisy += np.random.normal(0, noise_level, img.shape)
    noisy = np.clip(noisy, 0, 255)

    rows, cols = img.shape
    pts1 = np.float32([[0, 0], [cols - 1, 0], [0, rows - 1]])
    pts2 = np.float32([[0, 0], [cols - 1, 0], [deform_shift, rows - 1]])
    M = cv2.getAffineTransform(pts1, pts2)
    warped = cv2.warpAffine(noisy, M, (cols, rows), borderMode=cv2.BORDER_REPLICATE)

    return warped.astype(np.uint8)


def plot_similar_images(title, query_img, db_images, similarities):
    fig, axes = plt.subplots(2, 4, figsize=(12, 6))
    fig.suptitle(title, fontsize=16, fontweight="bold")

    highest = np.max(similarities)

    for i in range(4):
        axes[0, i].axis("off")
    axes[0, 1].imshow(query_img, cmap="gray")
    axes[0, 1].set_title("Query", fontsize=12)

    for i, (img, sim) in enumerate(zip(db_images, similarities)):
        axes[1, i].imshow(img, cmap="gray")
        axes[1, i].set_title(f"Similarity: {sim:.4f}", fontsize=10)
        if sim == highest:
            axes[1, i].set_title(
                f"BEST (Similarity: {sim:.4f})", fontsize=10, color="green"
            )
        axes[1, i].axis("off")

    plt.tight_layout()
    plt.show()


def compare_with_db(query_descriptor, db_dwt_descriptors, db_labels):
    print(f"Descriptor size: {query_descriptor.shape}")
    sims = cosine_similarity([query_descriptor], db_dwt_descriptors)[0]
    for idx, score in enumerate(sims):
        print(f"Similarity with {db_labels[idx]}: {score:.4f}")
    return sims


def extract_fft_descriptor(img, keep_fraction=0.25):
    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)
    mag = np.abs(fshift)
    log_mag = np.log1p(mag)

    h, w = log_mag.shape
    center_h, center_w = h // 2, w // 2
    size_h = int(h * keep_fraction / 2)
    size_w = int(w * keep_fraction / 2)
    cropped = log_mag[
        center_h - size_h // 2 : center_h + size_h // 2,
        center_w - size_w // 2 : center_w + size_w // 2,
    ]

    descriptor = cropped.flatten()
    return descriptor / np.linalg.norm(descriptor)


def wavelet_decompose(img, level=2, keep=1):
    coeffs = pywt.wavedec2(img, wavelet="haar", level=level)
    cA = coeffs[0]

    parts = [cA.flatten()]
    for cH, cV, cD in coeffs[1:][:keep]:
        parts.append(cH.flatten())
        parts.append(cV.flatten())
        parts.append(cD.flatten())

    desc = np.concatenate(parts).astype(np.float32)
    return desc / np.linalg.norm(desc)


def merge_descriptors(desc1, desc2):
    return np.concatenate((desc1, desc2))


if __name__ == "__main__":
    db_labels = ["smile", "sad", "neutral", "surprise"]
    db_imgs = [create_emoticon(emotion) for emotion in db_labels]

    db_dwt_descriptors = np.array([wavelet_decompose(img) for img in db_imgs])
    db_fft_descriptors = np.array([extract_fft_descriptor(img) for img in db_imgs])
    db_merged_descriptors = np.array(
        [
            merge_descriptors(dwt, fft)
            for dwt, fft in zip(db_dwt_descriptors, db_fft_descriptors)
        ]
    )

    query_img = distort_image(create_emoticon("smile"))
    query_fft_descriptor = extract_fft_descriptor(query_img)
    query_dwt_descriptor = wavelet_decompose(query_img)
    query_merged_descriptor = merge_descriptors(
        query_dwt_descriptor, query_fft_descriptor
    )

    print("FFT Descriptor Similarities:")
    sims = compare_with_db(query_fft_descriptor, db_fft_descriptors, db_labels)
    plot_similar_images("FFT", query_img, db_imgs, sims)

    print("\nWavelet Descriptor Similarities:")
    sims = compare_with_db(query_dwt_descriptor, db_dwt_descriptors, db_labels)
    plot_similar_images("Wavelet", query_img, db_imgs, sims)

    print("\nHybrid Descriptor Similarities:")
    sims = compare_with_db(query_merged_descriptor, db_merged_descriptors, db_labels)
    plot_similar_images("Hybrid", query_img, db_imgs, sims)
