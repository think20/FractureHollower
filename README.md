# FractureHollower

A companion tool for [HexaSimulator](https://github.com/think20/HexaSimulator) that
decides which simulated fibre breaks would actually be visible on an XCT scan of a
failed specimen.

## Why

A CT scan of a broken specimen only contains the half that is left after testing, and
only the breaks that lie below the (rough) fracture surface. A simulation, by
contrast, produces breaks throughout the whole gauge volume. Comparing the two
without filtering overestimates the observed count by roughly an order of magnitude.
This tool applies the three-step visibility criterion described in the paper:

1. **Axial alignment** — shift the simulated breaks by \(z_0\) so that the fracture
   surface of the simulation matches that of the scan;
2. **Rotational alignment** — rotate in \(\theta\) so that the lowest sector of the
   height map coincides with the crack-initiation sector;
3. **Point-by-point test** — look up the local fracture-surface height (searching up
   to ±8 pixels to fill voids in the CT reconstruction) and drop every break that
   lies above it.

The surviving breaks are written to `RemainingFractures.csv`; these are the ones that
should be compared with the XCT count.

## Build

```
msbuild FractureHollower.sln /p:Configuration=Release /p:Platform=x64
```

## Run

```
FractureHollower.exe <surface> <ZOffsetUm> <RotateDeg> [fracturesDir] [fracturesFile]
```

| argument | meaning |
|---|---|
| `surface` | name of the surface-height dataset, e.g. `F2Report` or `F3Report` |
| `ZOffsetUm` | axial alignment depth \(z_0\) in μm (e.g. 118 for F2, 128 for F3) |
| `RotateDeg` | rotational registration angle in degrees (e.g. 146 for F2, 70 for F3) |
| `fracturesDir` | optional; directory holding the fracture list (default as compiled) |
| `fracturesFile` | optional; fracture file name (default `Fractures.csv`) |

Example:

```
FractureHollower.exe F2Report 118 146 . Fractures.csv
```

Input format is the `Fractures.csv` written by HexaSimulator: one block per test
(`TestN:` followed by a count and then the break records).

## License

MIT — see [LICENSE](LICENSE).
