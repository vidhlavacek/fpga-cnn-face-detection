from PIL import Image
import numpy as np
import torch

def preprocess_own_image(image_path, image_size=320):
    original_image = Image.open(image_path).convert("RGB")
    original_np = np.array(original_image)

    original_height, original_width = original_np.shape[:2]

    scale = min(
        image_size / original_width,
        image_size / original_height
    )

    resized_width = int(round(original_width * scale))
    resized_height = int(round(original_height * scale))

    resized_image = original_image.resize(
        (resized_width, resized_height),
        Image.Resampling.BILINEAR
    )

    resized_np = np.array(resized_image)

    pad_left = (image_size - resized_width) // 2
    pad_top = (image_size - resized_height) // 2

    canvas = np.full(
        (image_size, image_size, 3),
        fill_value=114,
        dtype=np.uint8
    )

    canvas[
        pad_top:pad_top + resized_height,
        pad_left:pad_left + resized_width
    ] = resized_np

    image_tensor = (
        torch.from_numpy(canvas)
        .permute(2, 0, 1)
        .float() / 255.0
    )

    transform_info = {
        "original_width": original_width,
        "original_height": original_height,
        "scale": scale,
        "pad_left": pad_left,
        "pad_top": pad_top
    }

    return original_image, image_tensor, transform_info