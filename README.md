# FPGA ubrzana obrada slike primjenom konvolucijskih neuronskih mreža

Detekcija lica (anchor-free FCOS) s hardverskim ubrzanjem konvolucijskog bloka
na FPGA platformi PYNQ-Z2. Završni rad, Tehnički fakultet u Rijeci.

## Tehnologije

- **Python, PyTorch, NumPy** — treniranje modela i inferencija na pločici
- **C++, Vitis HLS** — izvorni kod IP jezgre i sinteza u RTL
- **Vivado** — integracija IP jezgre u block design
- **PYNQ-Z2** (Zynq-7000 SoC) — ciljna platforma

## Struktura

- **`model/`** — PyTorch model, treniranje na WIDER FACE skupu i izvoz težina
- **`accelerator_vitis/`** — izvorni C++ kod IP jezgre (prije HLS-a) i Vitis HLS konfiguracija
- **`accelerator_IP/`** — IP jezgra spakirana Vitis HLS-om (generirani RTL, driveri)
- **`vivado_design/`** — Vivado projekt s block designom (PYNQ base overlay + IP jezgra)
- **`accelerator_bitstream/`** — izgrađeni bitstream (`.bit`) i opis hardvera (`.hwh`)
- **`pynq/`** — kod koji se izvodi na PYNQ-Z2: NumPy inferencija, upravljanje IP jezgrom i overlay
