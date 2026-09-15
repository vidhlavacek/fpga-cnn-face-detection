import numpy as np

def load_weights(weights_path):
    
    weights = np.load(weights_path)
    
    return weights

