import numpy as np

def reLU(input):
    return np.maximum(input, 0.0).astype(np.float32)