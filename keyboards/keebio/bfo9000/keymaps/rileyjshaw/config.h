#pragma once

// #define USE_I2C

/* Select hand configuration */
// #define MASTER_RIGHT
// #define EE_HANDS

#undef MANUFACTURER
#define MANUFACTURER Orchid

#undef PRODUCT
#define PRODUCT MIDI Grid

#undef DESCRIPTION
#define DESCRIPTION An isomorphic / orthogonal MIDI controller

#define MIDI_BASIC
#define MIDI_ADVANCED
#define MIDI_TONE_KEYCODE_OCTAVES 6

#define SEQUENCER_STEPS 32

#define USB_MAX_POWER_CONSUMPTION 100
