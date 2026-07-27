# Roadmap and extension guide

## Version 2: one-button control

Connect a momentary button between digital pin 2 and GND and configure
`INPUT_PULLUP`. Use a 20-50 ms software debounce timer without `delay()`.

Planned gestures:

- Short press: next mode
- Double press: random mode
- Long press: toggle automatic cycling
- Very long press: reset runtime settings

Track raw/stable state, last change, press time, click count, and click timeout in
a fixed `ButtonState` structure.

## Version 3: microphone amplitude

MAX9814 offers automatic gain control; MAX4466 provides adjustable analog gain.
A basic system samples for a short window, calculates peak-to-peak amplitude,
subtracts a calibrated noise floor, smooths the value, and maps it to animation
intensity. This measures volume-like amplitude, not a frequency spectrum or a
reliable beat by itself.

```text
microphone -> sampling -> DC/peak-to-peak handling -> noise floor
           -> smoothing -> normalization -> visual parameter
```

A blocking 20 ms sample window is acceptable for an experiment but should be
replaced with scheduled sampling for smooth display updates.

## FFT design

Use ESP32, Raspberry Pi Pico, or Teensy for real spectral analysis. Define the
sample rate and sample count first; the Nyquist limit is half the sample rate,
and FFT-bin width is sample rate divided by sample count. Remove DC, apply a
window function, compute the FFT, group bins into bass/mid/treble bands, smooth
them, and synchronize results with display frames. Audio buffers and FFT
workspace make the Uno/Nano a poor default for this stage.

## Version 4: portable power

Add a protected LiPo cell, compatible charger such as an appropriately wired
TP4056 module, regulator if board/display voltages require one, physical switch,
insulation, and strain relief.

Never short, puncture, or charge a damaged LiPo. Do not leave conductive
contacts exposed or assume a bare cell can directly power every board.

## Enclosure

Measure the actual assembled hardware rather than relying on nominal module
dimensions. Allow an OLED viewing window, USB access, switch/button openings,
wire bend radius, mounting points, strain relief, and no pressure on OLED glass.
Prototype in cardboard or a project box before producing a 3D-printed or
laser-cut enclosure.

## Further software

- Favorite/freeze mode and speed control
- EEPROM settings with write-wear control
- Brightness/dim scheduling
- Microphone auto-calibration
- Beat-triggered particle emission
- ESP32 FFT bass/mid/treble mapping
- Host-side framebuffer simulation and golden-image tests
