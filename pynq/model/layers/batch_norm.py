import numpy as np

def batch_norm(x, gamma, beta, running_mean, running_var, eps=1e-5):
    """
    BatchNorm u inference načinu.
    Koristi spremljene running_mean i running_var iz PyTorcha.
    """

    gamma = gamma.reshape(1, -1, 1, 1)
    beta = beta.reshape(1, -1, 1, 1)
    running_mean = running_mean.reshape(1, -1, 1, 1)
    running_var = running_var.reshape(1, -1, 1, 1)

    return gamma * (x - running_mean) / np.sqrt(running_var + eps) + beta