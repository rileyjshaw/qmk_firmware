# Stargazer

_An isomorphic / orthogonal MIDI controller._

See [`keyboards/keebio/bfo9000/keymaps/rileyjshaw/keymap.c`](/keyboards/keebio/bfo9000/keymaps/rileyjshaw/keymap.c) for detailed keymap and layer definitions.

## Hardware

- [BFO-9000 108-key split ortholinear keyboard](https://keeb.io/products/bfo-9000-keyboard-customizable-full-size-split-ortholinear?variant=8116065271914) (add a Pro Micro and Elite-C)
- [Gateron Silent Black keyswitches (110)](https://novelkeys.xyz/)
- [Assorted colored keycaps](https://kbdfans.com/products/dsa-blank-keycaps-1u-10pcs?_pos=9&_sid=5bc264ef0&_ss=r)
- [Upgraded keycaps](https://ramaworks.store/products/grid-set-a?variant=16185116065835)

## Installation

First, ensure you have the prerequisite QMK software installed. With
[Homebrew](https://brew.sh/) installed, run the following in your Terminal:

```.sh
brew install qmk/qmk/qmk
brew tap homebrew/cask-drivers
brew install --cask qmk-toolbox
```

You’ll also need to install Git if you haven’t already:

```.sh
brew install git
```

To get the code, `cd` to your desired directory and clone this repository:

```.sh
git clone -b bfo-9000-midi git@github.com:rileyjshaw/qmk_firmware.git
cd qmk_firmware
```

Compile the firmware:

```.sh
qmk compile -kb keebio/bfo9000 -km rileyjshaw
```

Finally, plug in your Arduino-based keyboard and flash the firmware you just
compiled using the QMK Toolbox app. It should be called
`keebio_bfo9000_rileyjshaw.hex` in the root `qmk_firmware` directory.

_Note: It should also be possible to flash the firmware from the command line
using the following command, but it’s never worked for me:_

```.sh
# You should not need to run this if you already used QMK Toolbox.
qmk flash -kb keebio/bfo9000 -km rileyjshaw
```

## Editing the firmware

To edit the firmware, open the `keyboards/keebio/bfo9000/keymaps/rileyjshaw/`
directory in your editor, make some changes, then run the steps above to load
it onto your keyboard.

## References

- [BFO-9000 build guide](https://docs.keeb.io/iris-rev2-build-guide)
- [The QMK Tutorial (getting started)](https://docs.qmk.fm/#/newbs)
- [QMK MIDI reference](https://docs.qmk.fm/#/feature_midi)
- [QMK sequencer reference](https://docs.qmk.fm/#/feature_sequencer)
- [MIDI message specification](https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message)
- [MIDI CC List](https://professionalcomposers.com/midi-cc-list/)
- [Might be important for split keyboard](https://docs.qmk.fm/#/feature_split_keyboard)
- [midiphy MatriX](https://www.midiphy.com/en/matrix/)
- [Waffletone docs](https://github.com/cinnamon-bun/waffletone)
