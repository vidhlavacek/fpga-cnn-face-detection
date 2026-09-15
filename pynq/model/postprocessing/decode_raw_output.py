import numpy as np

from .math_functions import sigmoid, softplus
from .box_overlap import nms, box_iou_one_to_many

def decode_predictions(
    prediction,
    image_size=320,
    feature_size=40,
    score_threshold=0.05,
    nms_iou_threshold=0.4
):
    """
    prediction: (1, 6, 40, 40) ili (6, 40, 40)
    returns:
        boxes:  (N, 4), redoslijed [x1, y1, x2, y2]
        scores: (N,)
    """

    if prediction.ndim == 4:
        prediction = prediction[0]

    assert prediction.shape == (6, feature_size, feature_size), (
        f"Očekivan oblik (6, {feature_size}, {feature_size}), "
        f"dobiven: {prediction.shape}"
    )

    stride = image_size / feature_size

    face_probability = sigmoid(prediction[0])
    bbox_prediction = softplus(prediction[1:5])
    centerness_probability = sigmoid(prediction[5])

    scores_map = face_probability * centerness_probability

    y_indices, x_indices = np.where(
        scores_map >= score_threshold
    )

    if len(y_indices) == 0:
        return (
            np.empty((0, 4), dtype=np.float32),
            np.empty((0,), dtype=np.float32)
        )

    scores = scores_map[y_indices, x_indices]

    left = bbox_prediction[0, y_indices, x_indices] * image_size
    top = bbox_prediction[1, y_indices, x_indices] * image_size
    right = bbox_prediction[2, y_indices, x_indices] * image_size
    bottom = bbox_prediction[3, y_indices, x_indices] * image_size

    center_x = (x_indices.astype(np.float32) + 0.5) * stride
    center_y = (y_indices.astype(np.float32) + 0.5) * stride

    x1 = center_x - left
    y1 = center_y - top
    x2 = center_x + right
    y2 = center_y + bottom

    boxes = np.stack([x1, y1, x2, y2], axis=1)

    boxes[:, [0, 2]] = np.clip(
        boxes[:, [0, 2]],
        0,
        image_size - 1
    )

    boxes[:, [1, 3]] = np.clip(
        boxes[:, [1, 3]],
        0,
        image_size - 1
    )

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

    return (
        boxes[keep_indices].astype(np.float32),
        scores[keep_indices].astype(np.float32)
    )