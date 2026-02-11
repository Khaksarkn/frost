# FROST ❄️

**FROST** is a MOOSE-based application for **thermal–hydraulic (TH) modeling of saturated soils under freeze–thaw conditions**.  
It is designed to simulate ground thermal evolution with physics of **water–ice phase change** and **latent heat effects**, making it suitable for cold-region and permafrost-related studies.

---

## Core Capabilities

- Fully coupled **thermal–hydraulic (TH)** formulation
- Modeling of **water–ice phase transformation**
- Latent heat effects during freezing and thawing
- Flexible constitutive options for:
  - Soil Freezing Characteristic Curves (SFCC)
  - Relative permeability formulations
  - Thermal conductivity interpretations
- Designed for **saturated porous media**

---

## Typical Applications

- Freeze–thaw processes in soils
- Cold-region geotechnical problems
- Ground thermal response under imposed thermal loads
- Process-based studies of frozen ground evolution

---

## Framework & Dependencies

FROST is developed using the **MOOSE Framework**.

> **Note**  
> MOOSE is under continuous development. Updates to the framework may affect FROST functionality, and test files may require re-golding depending on the MOOSE version used.

---

## Getting Started

1. Install the MOOSE Framework: https://mooseframework.inl.gov/getting_started/installation/index.html
2. Clone this repository into your `projects/` directory.
3. Compile the application:

```bash
cd ~/projects/frost
make -j4
```

4. Run tests

```bash
run_tests -j4
```

5. Create your own input files and run. e.g.:

```bash
frost-opt -i ./problems/YourInputFile.i
```

or using parallel processing

```bash
mpiexec -n 4 ./frost-opt -i ./problems/YourInputFile.i
```

See https://mooseframework.inl.gov/moose/getting_started/new_users.html for starting a new application.

## Authors

- **Kian Khaksar**  
  Concordia University, Montreal, QC, Canada  

- **Biao Li**  
  Concordia University, Montreal, QC, Canada

## Disclaimer

This software is provided for research and educational purposes only and is supplied **“as is”**, without warranty of any kind. Users are responsible for independently validating results for their specific applications. Please see GPL-3.0 licence for details.
