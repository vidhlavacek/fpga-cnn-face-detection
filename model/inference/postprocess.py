import torch
import torch.nn.functional as F
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np


def box_iou_one_to_many(box, boxes):
    x1 = torch.maximum(box[0], boxes[:, 0])
    y1 = torch.maximum(box[1], boxes[:, 1])
    x2 = torch.minimum(box[2], boxes[:, 2])
    y2 = torch.minimum(box[3], boxes[:, 3])

    intersection_width = (x2 - x1).clamp(min=0)
    intersection_height = (y2 - y1).clamp(min=0)

    intersection = intersection_width * intersection_height

    box_area = (
        (box[2] - box[0]).clamp(min=0) *
        (box[3] - box[1]).clamp(min=0)
    )

    boxes_area = (
        (boxes[:, 2] - boxes[:, 0]).clamp(min=0) *
        (boxes[:, 3] - boxes[:, 1]).clamp(min=0)
    )

    union = box_area + boxes_area - intersection

    return intersection / union.clamp(min=1e-6)


def nms(boxes, scores, iou_threshold=0.4):
    if len(boxes) == 0:
        return torch.empty(0, dtype=torch.long, device=boxes.device)

    order = scores.argsort(descending=True)
    keep = []

    while len(order) > 0:
        current_index = order[0]
        keep.append(current_index)

        if len(order) == 1:
            break

        remaining_indices = order[1:]

        ious = box_iou_one_to_many(
            boxes[current_index],
            boxes[remaining_indices]
        )

        order = remaining_indices[ious <= iou_threshold]

    return torch.tensor(keep, dtype=torch.long, device=boxes.device)


@torch.no_grad()
def decode_predictions(
    prediction,
    device,
    image_size=320,
    feature_size=40,
    score_threshold=0.05,
    nms_iou_threshold=0.4
):
    stride = image_size / feature_size

    face_probability = torch.sigmoid(prediction[0])
    bbox_prediction = F.softplus(prediction[1:5])
    centerness_probability = torch.sigmoid(prediction[5])

    scores_map = face_probability * centerness_probability

    y_indices, x_indices = torch.where(
        scores_map >= score_threshold
    )

    if len(y_indices) == 0:
        empty_boxes = torch.empty(
            (0, 4),
            dtype=torch.float32,
            device=device
        )

        empty_scores = torch.empty(
            (0,),
            dtype=torch.float32,
            device=device
        )

        return empty_boxes, empty_scores

    scores = scores_map[y_indices, x_indices]

    left = bbox_prediction[0, y_indices, x_indices] * image_size
    top = bbox_prediction[1, y_indices, x_indices] * image_size
    right = bbox_prediction[2, y_indices, x_indices] * image_size
    bottom = bbox_prediction[3, y_indices, x_indices] * image_size

    center_x = (x_indices.float() + 0.5) * stride
    center_y = (y_indices.float() + 0.5) * stride

    x1 = center_x - left
    y1 = center_y - top
    x2 = center_x + right
    y2 = center_y + bottom

    boxes = torch.stack([x1, y1, x2, y2], dim=1)

    boxes[:, [0, 2]] = boxes[:, [0, 2]].clamp(0, image_size - 1)
    boxes[:, [1, 3]] = boxes[:, [1, 3]].clamp(0, image_size - 1)

    valid_boxes = (
        (boxes[:, 2] > boxes[:, 0]) &
        (boxes[:, 3] > boxes[:, 1])
    )

    boxes = boxes[valid_boxes]
    scores = scores[valid_boxes]

    keep_indices = nms(
        boxes,
        scores,
        iou_threshold=nms_iou_threshold
    )

    return boxes[keep_indices], scores[keep_indices]

def boxes_to_original_image(predicted_boxes, transform_info):
    if len(predicted_boxes) == 0:
        return predicted_boxes.cpu()

    boxes = predicted_boxes.detach().cpu().clone()

    scale = transform_info["scale"]
    pad_left = transform_info["pad_left"]
    pad_top = transform_info["pad_top"]

    original_width = transform_info["original_width"]
    original_height = transform_info["original_height"]

    boxes[:, [0, 2]] = (
        boxes[:, [0, 2]] - pad_left
    ) / scale

    boxes[:, [1, 3]] = (
        boxes[:, [1, 3]] - pad_top
    ) / scale

    boxes[:, [0, 2]] = boxes[:, [0, 2]].clamp(
        0,
        original_width - 1
    )

    boxes[:, [1, 3]] = boxes[:, [1, 3]].clamp(
        0,
        original_height - 1
    )

    return boxes