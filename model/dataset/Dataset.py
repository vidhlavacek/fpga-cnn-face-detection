#Wider Face dataset


#Imports
from pathlib import Path
from PIL import Image
import numpy as np
import torch
from torch.utils.data import Dataset, DataLoader

'''
Annotations format

Image name
Number of faces
[4 bbox values] [other attribute values]
'''
def parse_wider_annotations(annotation_file):
    annotation_file = Path(annotation_file)
    records = []

    dummy_box_line = "0 0 0 0 0 0 0 0 0 0"

    with annotation_file.open("r", encoding="utf-8") as f:
        while True:
            #read line by line
            image_relative_path = f.readline()

            #break condition (end of file)
            if not image_relative_path:
                break 

            #clear whitespaces start and end of line
            image_relative_path = image_relative_path.strip()

            #case if empty line
            if not image_relative_path:
                continue

            #read number of faces
            face_count_line = f.readline().strip()

            #for testing, REMOVE LATER
            if not face_count_line.isdigit():
                raise ValueError(
                    f"Neispravan broj lica za sliku: {image_relative_path}\n"
                    f"Pročitani redak: {face_count_line!r}"
                )

            #number of faces to integer
            face_count = int(face_count_line)
            boxes = []

            #face_count=0, image attributes are zeros (dummy), boxes reamains empty, and readline becomes next image
            if face_count == 0:
                current_position = f.tell()
                next_line = f.readline().strip()

                if next_line != dummy_box_line:
                    f.seek(current_position)

            else:
                #reading and saving face bounding box values
                for _ in range(face_count):
                    line = f.readline().strip()

                    if not line:
                        continue

                    values = list(map(int, line.split()))
                    x, y, width, height = values[:4]

                    # Invalid box check
                    if width <= 0 or height <= 0:
                        continue

                    #converting bounding box values from x1,y1,width,height to x1,y1,x2,y2 format
                    x1 = x
                    y1 = y
                    x2 = x + width
                    y2 = y + height

                    boxes.append([x1, y1, x2, y2])

            boxes = np.array(boxes, dtype=np.float32).reshape(-1, 4)

            records.append({
                "image_path": image_relative_path,
                "boxes": boxes
            })

    return records


#Dataset Class
class WiderFaceDataset(Dataset):
    def __init__(self, image_root, annotation_file, image_size=320):
        self.image_root = Path(image_root)
        self.image_size = image_size
        self.records = parse_wider_annotations(annotation_file)

    def __len__(self):
        return len(self.records)

    def __getitem__(self, index):
        record = self.records[index]

        #create image path and save bounding boxes values
        image_path = self.image_root / record["image_path"]
        boxes = record["boxes"].copy()

        #convert image to rgb and then to numpy array
        image = Image.open(image_path).convert("RGB")
        image = np.array(image)

        #scaling the image to desired size (padded scaling)
        original_height, original_width = image.shape[:2]

        scale = min(
            self.image_size / original_width,
            self.image_size / original_height
        )

        resized_width = int(round(original_width * scale))
        resized_height = int(round(original_height * scale))

        #resize image with calculated scaling factor
        resized_image = Image.fromarray(image).resize(
            (resized_width, resized_height),
            Image.Resampling.BILINEAR
        )

        resized_image = np.array(resized_image)

        #padding creation
        pad_left = (self.image_size - resized_width) // 2
        pad_top = (self.image_size - resized_height) // 2

        canvas = np.full(
            (self.image_size, self.image_size, 3),
            fill_value=114,
            dtype=np.uint8
        )

        #pasting the picture to canvas, resoult is resiszed image with padding
        canvas[
            pad_top:pad_top + resized_height,
            pad_left:pad_left + resized_width
        ] = resized_image

        #resizeing bounding box values
        if len(boxes) > 0:
            boxes[:, [0, 2]] = boxes[:, [0, 2]] * scale + pad_left
            boxes[:, [1, 3]] = boxes[:, [1, 3]] * scale + pad_top

            boxes[:, [0, 2]] = np.clip(
                boxes[:, [0, 2]], 0, self.image_size - 1
            )
            boxes[:, [1, 3]] = np.clip(
                boxes[:, [1, 3]], 0, self.image_size - 1
            )

        #converting images and boxes to tensors
        image_tensor = torch.from_numpy(canvas).permute(2, 0, 1).float() / 255.0
        boxes_tensor = torch.from_numpy(boxes).float()

        return image_tensor, boxes_tensor

# For dataloader batch loading of images, explains how data is loaded when data loader is called
def detection_collate(batch):
    images, boxes = zip(*batch)

    images = torch.stack(images, dim=0)

    return images, list(boxes)

