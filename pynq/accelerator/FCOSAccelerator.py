import time
import numpy as np
from pynq import allocate


class FCOSAccelerator:
 
    FRAC_BITS = 10
    SCALE = 1 << FRAC_BITS
    INT_BITS = 6
    Q_LIMIT = 2 ** (INT_BITS - 1)          
    Q_MIN, Q_MAX = -(1 << 15), (1 << 15) - 1 

    BUF_SPEC = {
        #feature map buffers (gmem0)
        "input":    (3 * 320 * 320,),
        "scratch1": (16 * 160 * 160,),
        "scratch2": (24 * 80 * 80,),
        "scratch3": (48 * 40 * 40,),
        "scratch4": (48 * 40 * 40,),
        "output":   (6 * 40 * 40,),
        #weights and biases (gmem1)
        "stem_weights":   (16 * 3 * 3 * 3,),
        "stem_bias":      (16,),
        "block1_weights": (24 * 16 * 3 * 3,),
        "block1_bias":    (24,),
        "block2_weights": (48 * 24 * 3 * 3,),
        "block2_bias":    (48,),
        "head0_weights":  (48 * 48 * 3 * 3,),
        "head0_bias":     (48,),
        "head1_weights":  (6 * 48 * 1 * 1,),
        "head1_bias":     (6,),
    }

    NPZ_KEY_MAP = {
        "stem_weights":   "stem.layers.weight",
        "stem_bias":      "stem.layers.bias",
        "block1_weights": "block1.layers.weight",
        "block1_bias":    "block1.layers.bias",
        "block2_weights": "block2.layers.weight",
        "block2_bias":    "block2.layers.bias",
        "head0_weights":  "head.0.layers.weight",
        "head0_bias":     "head.0.layers.bias",
        "head1_weights":  "head.1.weight",
        "head1_bias":     "head.1.bias",
    }

    REG_NAME_MAP = {
        "input":          "input_r",
        "output":         "output_r",
        "stem_weights":   "stem_weights",
        "stem_bias":      "stem_bias",
        "block1_weights": "block1_weights",
        "block1_bias":    "block1_bias",
        "block2_weights": "block2_weights",
        "block2_bias":    "block2_bias",
        "head0_weights":  "head0_weights",
        "head0_bias":     "head0_bias",
        "head1_weights":  "head1_weights",
        "head1_bias":     "head1_bias",
        "scratch1":       "scratch1",
        "scratch2":       "scratch2",
        "scratch3":       "scratch3",
        "scratch4":       "scratch4",
    }

    OUTPUT_SHAPE = (6, 40, 40)

    def __init__(self, ip, weights_npz, verbose=True, timeout_s=10.0):
        self.verbose = verbose
        self.timeout_s = timeout_s
        self._closed = False

        self.ip = ip
        self._log("IP loaded")

        #allocate buffers
        self.buffers = {}
        for name, shape in self.BUF_SPEC.items():
            self.buffers[name] = allocate(shape=shape, dtype=np.int16)
        self._log(f"Allocated {len(self.buffers)} DMA bafera.")

        #load, check, quantization, write weights
        weights = np.load(weights_npz)
        for buf_name, npz_key in self.NPZ_KEY_MAP.items():
            arr = weights[npz_key]
            self._check_range(buf_name, arr)
            self._load_into(buf_name, arr)
        self._log("Weights and biases loaded")

        #buffer physical addresses to registers
        for buf_key, field_name in self.REG_NAME_MAP.items():
            self._set_ptr(field_name, self.buffers[buf_key])
        self._log("Buffer addresses written to registers")

    def run(self, input_np, timeout_s=None, return_raw=False):
        if self._closed:
            raise RuntimeError("FaceAccelerator is closed")

        arr = np.asarray(input_np, dtype=np.float64)
        if arr.ndim == 4 and arr.shape[0] == 1:
            arr = arr.squeeze(0)
        if arr.shape != (3, 320, 320):
            raise ValueError(
                f"Excpected (3,320,320) or (1,3,320,320), got {input_np.shape}"
            )

        self._check_range("input", arr)
        self._load_into("input", arr)

        elapsed = self._run_ip(timeout_s or self.timeout_s)
        self._log(f"IP finished in {elapsed * 1000:.2f} ms")

        self.buffers["output"].invalidate()
        raw = np.array(self.buffers["output"])
        out = self._from_fixed(raw).reshape(self.OUTPUT_SHAPE)

        if return_raw:
            return out, raw.reshape(self.OUTPUT_SHAPE)
        return out

    def close(self):
        if self._closed:
            return
        for buf in self.buffers.values():
            try:
                buf.freebuffer()
            except Exception:
                pass
        self.buffers = {}
        self._closed = True
        self._log("Buffers freed")

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def __del__(self):
        try:
            self.close()
        except Exception:
            pass

    def _log(self, msg):
        if self.verbose:
            print(f"[FaceAccelerator] {msg}")

    def _to_fixed(self, x):
        x = np.round(np.asarray(x, dtype=np.float64) * self.SCALE)
        x = np.clip(x, self.Q_MIN, self.Q_MAX)
        return x.astype(np.int16)

    def _from_fixed(self, x):
        return np.asarray(x).astype(np.float64) / self.SCALE

    def _check_range(self, name, arr):
        m = float(np.abs(arr).max())
        if m >= self.Q_LIMIT:
            self._log(
                f"{name} maxabs={m:.4f} >= limit +-{self.Q_LIMIT} "
                f"- values will be clipped during quantization"
            )
        else:
            self._log(f"{name:16s} maxabs={m:.4f}  limit=+-{self.Q_LIMIT}  OK")

    def _load_into(self, buf_name, arr_np):
        self.buffers[buf_name][:] = self._to_fixed(arr_np.flatten())
        self.buffers[buf_name].flush()

    def _set_ptr(self, field_name, buf):
        addr = buf.physical_address
        lo_reg = getattr(self.ip.register_map, f"{field_name}_1")
        hi_reg = getattr(self.ip.register_map, f"{field_name}_2")
        setattr(lo_reg, field_name, addr & 0xFFFFFFFF)
        setattr(hi_reg, field_name, (addr >> 32) & 0xFFFFFFFF)

    def _run_ip(self, timeout_s):
        self.ip.register_map.CTRL.AP_START = 1
        start_t = time.time()
        while not self.ip.register_map.CTRL.AP_DONE:
            if time.time() - start_t > timeout_s:
                raise TimeoutError(
                    "IP didn't finish"
                    "AP_START/AP_IDLE"
                )
            time.sleep(0.001)
        return time.time() - start_t