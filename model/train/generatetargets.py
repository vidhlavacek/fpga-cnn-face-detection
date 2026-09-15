import torch

'''
Transform bounding box from dataset to target which the model is learning.

In this case target is 6, 40, 40 (model output) 
'''

def generate_targets(
    boxes,
    image_size=320,
    feature_size=40,
    center_sampling_ratio=0.5
):
    stride = image_size / feature_size 
    device = boxes.device

    target = torch.zeros(
        (6, feature_size, feature_size),
        dtype=torch.float32,
        device=device
    )

    if len(boxes) == 0:
        return target

    y_indices, x_indices = torch.meshgrid(
        torch.arange(feature_size, device=device),
        torch.arange(feature_size, device=device),
        indexing="ij"
    )

    x_points = (x_indices.float() + 0.5) * stride
    y_points = (y_indices.float() + 0.5) * stride

    x1 = boxes[:, 0].view(-1, 1, 1)
    y1 = boxes[:, 1].view(-1, 1, 1)
    x2 = boxes[:, 2].view(-1, 1, 1)
    y2 = boxes[:, 3].view(-1, 1, 1)

    widths = x2 - x1
    heights = y2 - y1

    center_x = (x1 + x2) / 2
    center_y = (y1 + y2) / 2

    central_width = widths * center_sampling_ratio
    central_height = heights * center_sampling_ratio

    central_x1 = center_x - central_width / 2
    central_y1 = center_y - central_height / 2
    central_x2 = center_x + central_width / 2
    central_y2 = center_y + central_height / 2

    inside_central_area = (
        (x_points >= central_x1) &
        (x_points <= central_x2) &
        (y_points >= central_y1) &
        (y_points <= central_y2)
    )

    areas = widths * heights
    candidate_areas = torch.where(
        inside_central_area,
        areas,
        torch.full_like(areas, float("inf"))
    )

    smallest_area, assigned_box_index = candidate_areas.min(dim=0)

    positive_mask = torch.isfinite(smallest_area)

    if not positive_mask.any():
        return target

    assigned_boxes = boxes[assigned_box_index]

    assigned_x1 = assigned_boxes[..., 0]
    assigned_y1 = assigned_boxes[..., 1]
    assigned_x2 = assigned_boxes[..., 2]
    assigned_y2 = assigned_boxes[..., 3]

    left = x_points - assigned_x1
    top = y_points - assigned_y1
    right = assigned_x2 - x_points
    bottom = assigned_y2 - y_points

    left_normalized = left / image_size
    top_normalized = top / image_size
    right_normalized = right / image_size
    bottom_normalized = bottom / image_size

    centerness = torch.sqrt(
        (torch.minimum(left, right) / torch.maximum(left, right).clamp(min=1e-6)) 
        *
        (torch.minimum(top, bottom) / torch.maximum(top, bottom).clamp(min=1e-6))
    )

    target[0, positive_mask] = 1.0
    target[1, positive_mask] = left_normalized[positive_mask]
    target[2, positive_mask] = top_normalized[positive_mask]
    target[3, positive_mask] = right_normalized[positive_mask]
    target[4, positive_mask] = bottom_normalized[positive_mask]
    target[5, positive_mask] = centerness[positive_mask]

    return target