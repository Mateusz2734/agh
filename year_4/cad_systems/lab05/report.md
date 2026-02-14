# Systemy CAD/CAE - Zadanie 5

Mateusz Wala (*<mateuszwala@student.agh.edu.pl>*)

## 1. Zmiany wprowadzone w funkcji `bitmap_terrain`

![git diff](imgs/diff.png)

## 2. Kraj

Wykorzystano mapę wysokości Nepalu.

![Nepal](imgs/height.png)

![Granice](imgs/borders.png)

## 3. Wywołanie funkcji

Poniższy fragment kodu wywołuje funkcję `bitmap_terrain` (200 elementów i B-spline 4-go stopnia) oraz generuje klatki animacji.

```matlab
mkdir animation;

fig = figure();
bitmap_terrain("./height.png", 200, 4);

for i = 1:60
    figure(fig);
    view([6*i, 50]);
    saveas(fig, fullfile("animation", sprintf("%02d.png", i)));
end
```

Przykładowe klatki animacji:

![Klatka 7](imgs/007.png)

![Klatka 30](imgs/030.png)

![Klatka 60](imgs/060.png)

## 4. Tworzenie animacji

Animacja została utworzona za pomocą poniższego kodu w Pythonie:

```python
import argparse
from pathlib import Path
from sys import exit

from PIL import Image


def process_images(
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
        process_images(
            args.input_dir,
            args.output,
            args.duration,
        )
    except Exception as e:
        print(f"Error: {e}")
        exit(1)
    exit(0)
```

Skrypt został wykonany przy pomocy polecenia:

```bash
uv run main.py -i './animation' -o 'imgs/result.gif'
```
