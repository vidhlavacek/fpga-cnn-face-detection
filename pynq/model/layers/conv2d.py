import numpy as np

def conv2d(x, weight, bias=None, stride=1, padding=0):
    """
    x:      [B, Cin, H, W]
    weight: [Cout, Cin, Kh, Kw]
    bias:   [Cout] ili None
    """

    batch, cin, height, width = x.shape
    cout, weight_cin, kernel_h, kernel_w = weight.shape

    x_padded = np.pad(
        x,
        pad_width=((0, 0), (0, 0), (padding, padding), (padding, padding)),
        mode="constant"
    )

    output_h = (height + 2 * padding - kernel_h) // stride + 1
    output_w = (width + 2 * padding - kernel_w) // stride + 1

    stride_batch, stride_channel, stride_h, stride_w = x_padded.strides

    windows = np.lib.stride_tricks.as_strided(
        x_padded,
        shape=(batch, cin, output_h, output_w, kernel_h, kernel_w),
        strides=(
            stride_batch,
            stride_channel,
            stride_h * stride,
            stride_w * stride,
            stride_h,
            stride_w
        ),
        writeable=False
    )

    columns = windows.transpose(0, 2, 3, 1, 4, 5).reshape(
        batch * output_h * output_w,
        cin * kernel_h * kernel_w
    )

    # (Cout, Cin, Kh, Kw) -> (Cout, Cin*Kh*Kw)
    filters = weight.reshape(cout, cin * kernel_h * kernel_w)

    # Svaki red iz columns množi se sa svih 16 filtera.
    output = columns @ filters.T

    if bias is not None:
        output += bias.reshape(1, -1)

    # (B*Hout*Wout, Cout) -> (B, Cout, Hout, Wout)
    output = output.reshape(batch, output_h, output_w, cout)
    output = output.transpose(0, 3, 1, 2)

    return output.astype(np.float32)