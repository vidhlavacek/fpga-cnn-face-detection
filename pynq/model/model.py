import numpy as np

from .layers.conv_block import conv_block
from .layers.conv2d import conv2d

from .weights.load_weights import load_weights

class MiniFCOSFaceV1:
    def __init__(self, weights_path):
        self.weights = load_weights(weights_path)
         
    def __call__(self, x):
        return self.forward(x)
        
    def forward(self, x):
        stem = conv_block(
        x=x,
        weights=self.weights,
        conv_name="stem.layers.0.weight",
        bn_name="stem.layers.1",
        stride=2,
        padding=1
        )

        block1 = conv_block(
            x=stem,
            weights=self.weights,
            conv_name="block1.layers.0.weight",
            bn_name="block1.layers.1",
            stride=2,
            padding=1
        )

        block2 = conv_block(
            x=block1,
            weights=self.weights,
            conv_name="block2.layers.0.weight",
            bn_name="block2.layers.1",
            stride=2,
            padding=1
        )

        head_features = conv_block(
            x=block2,
            weights=self.weights,
            conv_name="head.0.layers.0.weight",
            bn_name="head.0.layers.1",
            stride=1,
            padding=1
        )

        output = conv2d(
            x=head_features,
            weight=self.weights["head.1.weight"],
            bias=self.weights["head.1.bias"],
            stride=1,
            padding=0
        )

        return output.astype(np.float32)