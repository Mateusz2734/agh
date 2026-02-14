import argparse
from pathlib import Path
from sys import exit

from PIL import Image


def create_gif_from_pngs(
    input_dir: str,
    output_path: str = None,
    duration: int = 100,
):
    input_path = Path(input_dir)
    output_path = Path(output_path)

    if not input_path.exists():
        raise ValueError(f"Directory does not exist: {input_dir}")

    if not input_path.is_dir():
        raise ValueError(f"Path is not a directory: {input_dir}")

    png_files = sorted(input_path.glob("*.png"))

    if not png_files:
        raise ValueError(f"No PNG files found in directory: {input_dir}")

    print(f"Found {len(png_files)} PNG files in {input_dir}")

    images = []
    for png_file in png_files:
        try:
            img = Image.open(png_file)
            images.append(img)
            print(f"Loaded: {png_file.name}")
        except Exception as e:
            print(f"Warning: Could not load {png_file.name}: {e}")

    if not images:
        raise ValueError("No valid images could be loaded")

    print(f"Creating GIF: {output_path}")
    images[0].save(
        output_path,
        save_all=True,
        append_images=images[1:],
        duration=duration,
        loop=0,
    )

    print(f"\nGIF created successfully: {output_path}")
    print(f"  - Frames: {len(images)}")
    print(f"  - Duration per frame: {duration}ms")
    print(f"  - Total duration: {len(images) * duration / 1000:.2f}s")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-i",
        "--input_dir",
        required=True,
        type=str,
        help="Directory containing PNG files",
    )
    parser.add_argument(
        "-o",
        "--output",
        required=True,
        type=str,
        help="Output GIF file path",
    )
    parser.add_argument(
        "-d",
        "--duration",
        type=int,
        default=100,
        help="Duration of each frame in milliseconds (default: 100)",
    )

    args = parser.parse_args()

    try:
        create_gif_from_pngs(
            args.input_dir,
            args.output,
            args.duration,
        )
    except Exception as e:
        print(f"Error: {e}")
        exit(1)
    exit(0)
