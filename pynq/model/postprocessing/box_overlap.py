import numpy as np


def box_iou_one_to_many(box, boxes):
    x1 = np.maximum(box[0], boxes[:, 0])
    y1 = np.maximum(box[1], boxes[:, 1])
    x2 = np.minimum(box[2], boxes[:, 2])
    y2 = np.minimum(box[3], boxes[:, 3])

    intersection_width = np.maximum(x2 - x1, 0.0)
    intersection_height = np.maximum(y2 - y1, 0.0)

    intersection = intersection_width * intersection_height

    box_area = (
        np.maximum(box[2] - box[0], 0.0) *
        np.maximum(box[3] - box[1], 0.0)
    )

    boxes_area = (
        np.maximum(boxes[:, 2] - boxes[:, 0], 0.0) *
        np.maximum(boxes[:, 3] - boxes[:, 1], 0.0)
    )

    union = box_area + boxes_area - intersection

    return intersection / np.maximum(union, 1e-6)


def nms(boxes, scores, iou_threshold=0.4):
    if len(boxes) == 0:
        return np.empty((0,), dtype=np.int64)

    order = np.argsort(scores)[::-1]
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

    return np.array(keep, dtype=np.int64)