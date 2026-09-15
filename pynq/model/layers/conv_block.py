import numpy as np
from .conv2d import conv2d
from .batch_norm import batch_norm
from .relu import reLU

def conv_block(x, weights, conv_name, bn_name, stride, padding):
    """
    Jedan blok:
    Conv2D -> BatchNorm (inference) -> ReLU

    conv_name: ime Conv weight tenzora u .npz datoteci
    bn_name:   početak imena BatchNorm parametara
    """

    conv_output = conv2d(
        x=x,
        weight=weights[conv_name],
        bias=None,
        stride=stride,
        padding=padding
    )

    bn_output = batch_norm(
        x=conv_output,
        gamma=weights[f"{bn_name}.weight"],
        beta=weights[f"{bn_name}.bias"],
        running_mean=weights[f"{bn_name}.running_mean"],
        running_var=weights[f"{bn_name}.running_var"],
        eps=1e-5
    )
    
    reLU_output = reLU(bn_output)

    return reLU_output