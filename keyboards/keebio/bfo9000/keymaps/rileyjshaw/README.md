# MIDI Grid

_An isomorphic / orthogonal MIDI controller._

See [`keyboards/keebio/bfo9000/keymaps/rileyjshaw/keymap.c`](/keyboards/keebio/bfo9000/keymaps/rileyjshaw/keymap.c) for detailed keymap and layer definitions.

## Hardware

- [BFO-9000 108-key split ortholinear keyboard](https://keeb.io/products/bfo-9000-keyboard-customizable-full-size-split-ortholinear?variant=8116065271914) (add a Pro Micro and Elite-C)
- [Gateron Silent Black keyswitches (110)](https://novelkeys.xyz/)
- [Assorted colored keycaps](https://kbdfans.com/products/dsa-blank-keycaps-1u-10pcs?_pos=9&_sid=5bc264ef0&_ss=r)
- [Upgrade keycaps](https://ramaworks.store/products/grid-set-a?variant=16185116000299)

## Editing the firmware

```.sh
# Edit the files, then run…
qmk compile -kb keebio/bfo9000 -km rileyjshaw

# Plug in your Arduino-based keyboard, then run…
qmk flash -kb keebio/bfo9000 -km rileyjshaw
```

## References

- [BFO-9000 build guide](https://docs.keeb.io/iris-rev2-build-guide)
- [The QMK Tutorial (getting started)](https://docs.qmk.fm/#/newbs)
- [QMK MIDI reference](https://docs.qmk.fm/#/feature_midi)
- [QMK sequencer reference](https://docs.qmk.fm/#/feature_sequencer)
- [MIDI message specification](https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message)
- [MIDI CC List](https://professionalcomposers.com/midi-cc-list/)
- [Might be important for split keyboard](https://docs.qmk.fm/#/feature_split_keyboard)
- [Waffletone docs](https://github.com/cinnamon-bun/waffletone)
