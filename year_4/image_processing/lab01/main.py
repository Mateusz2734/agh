import cv2
import numpy as np
import matplotlib.pyplot as plt
import argparse
from sklearn.neighbors import KNeighborsClassifier
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
    return descriptor


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Emoticon Recognition")
    parser.add_argument("--metric", type=str, choices=["euclidean", "cosine"], default="euclidean", help="Distance metric")

    args = parser.parse_args()
    metric = args.metric

    db_labels = ["smile", "sad", "neutral", "surprise"]
    db_imgs = [distort_image(create_emoticon(emotion)) for emotion in db_labels]
    db_descriptors = np.array([extract_fft_descriptor(img) for img in db_imgs])

    query_img = create_emoticon("smile")
    query_descriptor = extract_fft_descriptor(query_img)

    # Matching
    if metric == "euclidean":
        dists = np.linalg.norm(db_descriptors - query_descriptor, axis=1)
        best_match_idx = np.argmin(dists)
        best_match_label = db_labels[best_match_idx]
        best_match_img = db_imgs[best_match_idx]
        score = dists[best_match_idx]

    elif metric == "cosine":
        sims = cosine_similarity([query_descriptor], db_descriptors)[0]
        best_match_idx = np.argmax(sims)
        best_match_label = db_labels[best_match_idx]
        best_match_img = db_imgs[best_match_idx]
        score = sims[best_match_idx]

    plt.figure(figsize=(8, 4))
    plt.subplot(1, 2, 1)
    plt.title("Query Image")
    plt.imshow(query_img, cmap="gray")
    plt.axis("off")

    plt.subplot(1, 2, 2)
    plt.title(f"Best Match: {best_match_label} (Score: {score:.2f})")
    plt.imshow(best_match_img, cmap="gray")
    plt.axis("off")

    plt.tight_layout()
    plt.show()