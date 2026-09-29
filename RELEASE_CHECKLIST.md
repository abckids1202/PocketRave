# PocketRave v1.0.0 release checklist

Record the board model, OLED model/address, supply voltage, Arduino AVR core
version, Adafruit library versions, date, and tester in the test log before
sign-off.

## Source and build gates

- [ ] Working tree is clean and the intended commit is on `main`.
- [ ] `VERSION` contains `1.0.0`.
- [ ] `pio run -e nanoatmega328` succeeds with no project warnings.
- [ ] `pio run -e i2cscanner` succeeds.
- [ ] `pio run -e oledtest` succeeds.
- [ ] GitHub Actions passes on the release commit.
- [ ] Flash/SRAM results and the 1 KB runtime framebuffer note are documented.

## Reference hardware gates

- [ ] Blink uploads to the Nano.
- [ ] Scanner detects the OLED at `0x3C` or `0x3D`.
- [ ] OLED test shows stable primitives and text.
- [ ] PocketRave startup completes and enters animation mode.
- [ ] All nine modes enter, exit, and re-enter without corruption.
- [ ] FPS is approximately 30 and mode duration is 8–12 seconds.
- [ ] Three power cycles complete without a reset loop or display artifact.
- [ ] A 60-minute soak test completes with no freeze, slowdown, reset, heat, or odor.

## Publication gates

- [ ] Update `CHANGELOG.md` if release notes changed.
- [ ] Create annotated Git tag `v1.0.0`.
- [ ] Push the branch and tag to GitHub.
- [ ] Publish the GitHub release with the source archive and test record.
