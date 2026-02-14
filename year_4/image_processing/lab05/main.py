from random import uniform

import cv2
import matplotlib.pyplot as plt
import numpy as np
import pywt
from sklearn.decomposition import PCA
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
    top_indices = np.argsort(-similarities)[:4]
    top_similarities = similarities[top_indices]
    top_images = [db_images[i] for i in top_indices]

    fig, axes = plt.subplots(2, 4, figsize=(12, 6))
    fig.suptitle(title, fontsize=16, fontweight="bold")

    highest = np.max(top_similarities)

    for i in range(4):
        axes[0, i].axis("off")
    axes[0, 1].imshow(query_img, cmap="gray")
    axes[0, 1].set_title("Query", fontsize=12)

    for i, (img, sim) in enumerate(zip(top_images, top_similarities)):
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


def pca_reduce(db_descriptors, n_components):
    pca = PCA(n_components=n_components, random_state=42)
    db_reduced = pca.fit_transform(db_descriptors)
    return pca, db_reduced


def transform_with_pca(pca, descriptors):
    return pca.transform(descriptors.reshape(1, -1))


def retrieve(db_reduced, query_reduced, k=4):
    sims = cosine_similarity([query_reduced], db_reduced)[0]
    idx_sorted = np.argsort(-sims)
    top_idx = idx_sorted[:k]
    return top_idx, sims[top_idx], sims


if __name__ == "__main__":
    np.random.seed(42)

    db_labels = ["smile", "sad", "neutral", "surprise"] * 10
    db_imgs = [
        distort_image(create_emoticon(emotion), uniform(15, 60), uniform(7, 20))
        for emotion in db_labels
    ]

    db_dwt_descriptors = np.array([wavelet_decompose(img) for img in db_imgs])
    db_fft_descriptors = np.array([extract_fft_descriptor(img) for img in db_imgs])
    db_hybrid_descriptors = np.array([merge_descriptors(dwt, fft) for dwt, fft in zip(db_dwt_descriptors, db_fft_descriptors)])

    query_img = create_emoticon("smile")
    query_fft_descriptor = extract_fft_descriptor(query_img)
    query_dwt_descriptor = wavelet_decompose(query_img)
    query_hybrid_descriptor = merge_descriptors(query_fft_descriptor, query_dwt_descriptor)

    n_components = 16
    k = 4

    # FFT PCA
    pca_fft, db_fft_reduced = pca_reduce(db_fft_descriptors, n_components=n_components)
    print(f"Shapes for DWT: original={db_fft_descriptors.shape}, after_pca={db_fft_reduced.shape}")
    query_fft_reduced = transform_with_pca(pca_fft, query_fft_descriptor)[0]
    top_idx_fft, top_sims_fft, all_sims_fft = retrieve(db_fft_reduced, query_fft_reduced, k=k)
    print(top_sims_fft)
    plot_similar_images("FFT PCA", query_img, db_imgs, all_sims_fft)

    # DWT PCA
    pca_dwt, db_dwt_reduced = pca_reduce(db_dwt_descriptors, n_components=n_components)
    print(f"Shapes for DWT: original={db_dwt_descriptors.shape}, after_pca={db_dwt_reduced.shape}")
    query_dwt_reduced = transform_with_pca(pca_dwt, query_dwt_descriptor)[0]
    top_idx_dwt, top_sims_dwt, all_sims_dwt = retrieve(db_dwt_reduced, query_dwt_reduced, k=k)
    print(top_sims_dwt)
    plot_similar_images("DWT PCA", query_img, db_imgs, all_sims_dwt)

    # Hybrid PCA
    pca, db_reduced = pca_reduce(db_hybrid_descriptors, n_components=n_components)
    print(f"Shapes for Hybrid: original={db_hybrid_descriptors.shape}, after_pca={db_reduced.shape}")
    query_reduced = transform_with_pca(pca, query_hybrid_descriptor)[0]
    top_idx, top_sims, all_sims = retrieve(db_reduced, query_reduced, k=k)
    print(top_sims)
    plot_similar_images("Hybrid + PCA", query_img, db_imgs, all_sims)
