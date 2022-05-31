#pragma once

// #define USE_I2C

/* Select hand configuration */
// #define MASTER_RIGHT
// #define EE_HANDS

#undef MANUFACTURER
#define MANUFACTURER Misery & Co.

#undef PRODUCT
#define PRODUCT Stargazer

#undef DESCRIPTION
#define DESCRIPTION An isomorphic / orthogonal MIDI keyboard

#define MIDI_BASIC
#define MIDI_ADVANCED
#define MIDI_TONE_KEYCODE_OCTAVES 6

#define SEQUENCER_STEPS 32

#define USB_MAX_POWER_CONSUMPTION 100

// Save some firmware space by turning off unused features.
#define NO_ACTION_TAPPING
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
