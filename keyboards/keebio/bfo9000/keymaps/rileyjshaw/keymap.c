#include QMK_KEYBOARD_H

extern MidiDevice midi_device;

enum layers {
    // These layers underlie the entire keyboard as interchangeable base layers. Since other
    // layers overlap these, they are referred to as the “left hand” layers.
    _LH_CHROMATIC,
    _LH_MAJOR,
    _LH_MINOR,
    // _LH_DRUM,
    // _LH_GUITAR,
    // _LH_PO,
    // _LH_SEQUENCER,

    _START_OF_LAYER_GROUPS,
    // Right hand.
    _RH_CHROMATIC = _START_OF_LAYER_GROUPS,
    _RH_MAJOR,
    _RH_MINOR,
    _RH_DRUM,
    _RH_PO,
    // _RH_GUITAR,
    // _RH_SEQUENCER,
    _END_OF_RH_LAYER_GROUP,

    // Left column.
    _LC_PERFORM = _END_OF_RH_LAYER_GROUP,
    _LC_TRANSPOSE,
    _LC_CCS,
    _LC_CCM,
    _LC_CCT,
    _LC_CHANNEL,
    _END_OF_LC_LAYER_GROUP,

    // Right column.
    _RC_PERFORM = _END_OF_LC_LAYER_GROUP,
    _RC_TRANSPOSE,
    _RC_CCS,
    _RC_CCM,
    _RC_CCT,
    _RC_CHANNEL,
    _END_OF_RC_LAYER_GROUP,
    _END_OF_LAYER_GROUPS = _END_OF_RC_LAYER_GROUP,

    // Other.
    _QWERTY = _END_OF_LAYER_GROUPS,

    // Command layer.
    _COMMAND_KEY,
    _COMMAND,
    _CONTROL
};

// Bitmasks represent which bits are contained within each layer group. For instance, if the
// first and second layer groups each contained three layers, the second bitmask would look like:
//
//   0b00000000000000000000000000111000
//
// The compliment can be obtained by using the `~` operator.
const layer_state_t RH_BITMASK = (1UL << _END_OF_RH_LAYER_GROUP) - (1UL << _START_OF_LAYER_GROUPS);
const layer_state_t LC_BITMASK = (1UL << _END_OF_LC_LAYER_GROUP) - (1UL << _END_OF_RH_LAYER_GROUP);
const layer_state_t RC_BITMASK = (1UL << _END_OF_RC_LAYER_GROUP) - (1UL << _END_OF_LC_LAYER_GROUP);

// Sequencer resolution and tempo defaults.
#define _SQ_RES_INIT 6  // SQ_RES_8, twice per beat.
#define _SQ_TMP_1 80
#define _SQ_TMP_2 120
#define _SQ_TMP_3 160

enum custom_keycodes {
    LOWER_OCTAVE_KEYCODES_START = SAFE_RANGE,
    MI_AsN2 = LOWER_OCTAVE_KEYCODES_START,
    MI_BbN2 = MI_AsN2,
    MI_BN2,
    MI_CN1,
    MI_CsN1,
    MI_DbN1 = MI_CsN1,
    MI_DN1,
    MI_DsN1,
    MI_EbN1 = MI_DsN1,
    MI_EN1,
    MI_FN1,
    MI_FsN1,
    MI_GbN1 = MI_FsN1,
    MI_GN1,
    MI_GsN1,
    MI_AbN1 = MI_GsN1,
    MI_AN1,
    MI_AsN1,
    MI_BbN1 = MI_AsN1,
    MI_BN1,
    LOWER_OCTAVE_KEYCODES_END = MI_BN1,

    HIGHER_OCTAVE_KEYCODES_START,
    MI_C6 = HIGHER_OCTAVE_KEYCODES_START,
    MI_Cs6,
    MI_Db6 = MI_Cs6,
    MI_D6,
    MI_Ds6,
    MI_Eb6 = MI_Ds6,
    MI_E6,
    // MI_F6,
    // MI_Fs6,
    // MI_Gb6 = MI_Fs6,
    // MI_G6,
    // MI_Gs6,
    // MI_Ab6 = MI_Gs6,
    HIGHER_OCTAVE_KEYCODES_END = MI_E6,

    NON_TRANSPOSED_CHANNEL_KEYCODES_START,
    MX_CH1 = NON_TRANSPOSED_CHANNEL_KEYCODES_START,
    MX_CH2,
    MX_CH3,
    MX_CH4,
    MX_CH5,
    MX_CH6,
    MX_CH7,
    MX_CH8,
    MX_CH9,
    MX_CH10,
    MX_CH11,
    MX_CH12,
    MX_CH13,
    MX_CH14,
    MX_CH15,
    MX_CH16,
    NON_TRANSPOSED_CHANNEL_KEYCODES_END = MX_CH16,

    NON_TRANSPOSED_KEYCODES_START,
    MI_X17 = NON_TRANSPOSED_KEYCODES_START,
    MI_X18,
    MI_X19,
    MI_X20,
    MI_X21,
    MI_X22,
    MI_X23,
    MI_X24,
    MI_X25,
    MI_X26,
    MI_X27,
    MI_X28,
    MI_X29,
    MI_X30,
    MI_X31,
    MI_X32,
    MI_X33,
    MI_X34,
    MI_X35,
    MI_X36,
    MI_X37,
    MI_X38,
    MI_X39,
    MI_X40,
    MI_X41,
    MI_X42,
    MI_X43,
    MI_X44,
    MI_X45,
    MI_X46,
    MI_X47,
    MI_X48,
    NON_TRANSPOSED_KEYCODES_END = MI_X48,

    // MIDI CC codes 102-119 are undefined in the spec, so we can use them
    // as general purpose controls.
    CC_KEYCODES_START,
    // Momentary.
    MI_CCM1 = CC_KEYCODES_START,
    MI_CCM2,
    MI_CCM3,
    MI_CCM4,
    MI_CCM5,
    MI_CCM6,
    MI_CCM7,
    MI_CCM8,
    MI_CCM9,
    CC_MOMENTARY_END = MI_CCM9,

    CC_TOGGLE_KEYCODES_START,
    MI_CCT1 = CC_TOGGLE_KEYCODES_START,
    MI_CCT2,
    MI_CCT3,
    MI_CCT4,
    MI_CCT5,
    MI_CCT6,
    MI_CCT7,
    MI_CCT8,
    MI_CCT9,
    CC_KEYCODES_END = MI_CCT9,

    SEQUENCER_KEYCODES_START,
    SQ_TCLR = SEQUENCER_KEYCODES_START,
    SQ_TMP1,
    SQ_TMP2,
    SQ_TMP3
};

void keyboard_post_init_user(void) {
    layer_on(_LC_PERFORM);
    layer_on(_COMMAND_KEY);

    sequencer_set_tempo(_SQ_TMP_2);
    sequencer_set_resolution(_SQ_RES_INIT);

    for (uint8_t i = 0; i < 8; ++i) {
        sequencer_config.track_notes[i] = MI_C2 + i;
    }
}

bool toggle_cc_state[16][CC_KEYCODES_END + 1 - CC_TOGGLE_KEYCODES_START] = { false };
int8_t non_transposed_channel = 7; // Send drums over MIDI Channel 8 by default.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t note_number = 0;
    uint8_t momentary_cc_number = 0;
    uint8_t toggle_cc_number = 0;
    uint8_t fixed_channel_number = 100;  // Invalid, so we ignore it unless its set.

    switch (keycode) {
        // MIDI note extensions outside of the regular octave range.
        case LOWER_OCTAVE_KEYCODES_START ... LOWER_OCTAVE_KEYCODES_END:
            note_number = keycode + 33 - LOWER_OCTAVE_KEYCODES_START;
            break;
        case HIGHER_OCTAVE_KEYCODES_START ... HIGHER_OCTAVE_KEYCODES_END:
            note_number = keycode + 119 - HIGHER_OCTAVE_KEYCODES_START;
            break;
        case NON_TRANSPOSED_CHANNEL_KEYCODES_START ... NON_TRANSPOSED_CHANNEL_KEYCODES_END:
            non_transposed_channel = keycode - NON_TRANSPOSED_CHANNEL_KEYCODES_START;
            break;
        case NON_TRANSPOSED_KEYCODES_START ... NON_TRANSPOSED_KEYCODES_END:
            note_number = keycode + 98 - NON_TRANSPOSED_KEYCODES_START - 12 * midi_config.octave - midi_config.transpose;
            fixed_channel_number = non_transposed_channel;
            break;
        case CC_KEYCODES_START ... CC_MOMENTARY_END:
            momentary_cc_number = keycode + 102 - CC_KEYCODES_START;
            break;
        case CC_TOGGLE_KEYCODES_START ... CC_KEYCODES_END:
            toggle_cc_number = keycode + 102 - CC_KEYCODES_START;
            break;

        // Deactivate all sequencer tracks.
        case SQ_TCLR:
            // A bit of a hack – ensure Track 0 is active, then call a function
            // that deactivates all tracks if 0 is active.
            sequencer_activate_track(0);
            sequencer_toggle_single_active_track(0);
            break;

        // Set sequencer tempo to a preset value.
        case SQ_TMP1:
            sequencer_set_tempo(_SQ_TMP_1);
            break;
        case SQ_TMP2:
            sequencer_set_tempo(_SQ_TMP_2);
            break;
        case SQ_TMP3:
            sequencer_set_tempo(_SQ_TMP_3);
    /*Amen*/break;

        default:
            return true;
    }

    uint8_t midi_channel = fixed_channel_number < 16 ? fixed_channel_number : midi_config.channel;
    if (note_number) {
        if (record->event.pressed) {
            midi_send_noteon(&midi_device, midi_channel, midi_compute_note(note_number), midi_config.velocity);
        } else {
            midi_send_noteoff(&midi_device, midi_channel, midi_compute_note(note_number), 0);
        }
    } else if (momentary_cc_number) {
        midi_send_cc(&midi_device, midi_channel, momentary_cc_number, record->event.pressed ? 127 : 0);
    } else if (toggle_cc_number && record->event.pressed) {
        bool toggle_state = !toggle_cc_state[midi_channel][toggle_cc_number - 102];
        toggle_cc_state[midi_channel][toggle_cc_number - 102] = toggle_state;
        midi_send_cc(&midi_device, midi_channel, toggle_cc_number, toggle_state ? 127 : 0);
    }

    return true;
}

layer_state_t prev_layer_state = 0;
layer_state_t layer_state_set_user(layer_state_t state) {
    layer_state_t changed_layer_bitmask = state ^ prev_layer_state;
    uint8_t changed_layer = get_highest_layer(changed_layer_bitmask);

    // If a layer is out of range or is being turned off, don’t do anything fancy.
    if (changed_layer >= _START_OF_LAYER_GROUPS && changed_layer <= _END_OF_LAYER_GROUPS && IS_LAYER_ON_STATE(state, changed_layer)) {
        // Clear all other layers in the layer group.
        if (changed_layer < _END_OF_RH_LAYER_GROUP) {
            state &= ~RH_BITMASK;
        } else if (changed_layer < _END_OF_LC_LAYER_GROUP) {
            state &= ~LC_BITMASK;
        } else if (changed_layer < _END_OF_RC_LAYER_GROUP) {
            state &= ~RC_BITMASK;
        }

        // Turn the changed layer back on.
        state |= changed_layer_bitmask;
    }

    prev_layer_state = state;
    return state;
}

// TODO: Generate left and right variants dynamically at compile time instead of copy/pasting.
/* Empty layer template for copy-pasting:

// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_LAYER_NAME] = LAYOUT( \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
),

*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// | 1     | 2     | 3    | 4     | 5     | 6    | 7     | 8    | 9     |||||| 10    | 11    | 12    | 13   | 14    | 15    | 16    | 17    | 18   |
[_LH_CHROMATIC] = LAYOUT( \
    MI_C2,  MI_Cs2, MI_D2, MI_Ds2, MI_E2,  MI_F2, MI_Fs2, MI_G2, MI_Gs2,      MI_A2,  MI_As2, MI_B2,  MI_C3, MI_Cs3, MI_D3,  MI_Ds3, MI_E3,  MI_F3, \
    MI_G1,  MI_Gs1, MI_A1, MI_As1, MI_B1,  MI_C2, MI_Cs2, MI_D2, MI_Ds2,      MI_E2,  MI_F2,  MI_Fs2, MI_G2, MI_Gs2, MI_A2,  MI_As2, MI_B2,  MI_C3, \
    MI_D1,  MI_Ds1, MI_E1, MI_F1,  MI_Fs1, MI_G1, MI_Gs1, MI_A1, MI_As1,      MI_B1,  MI_C2,  MI_Cs2, MI_D2, MI_Ds2, MI_E2,  MI_F2,  MI_Fs2, MI_G2, \
    MI_A,   MI_As,  MI_B,  MI_C1,  MI_Cs1, MI_D1, MI_Ds1, MI_E1, MI_F1,       MI_Fs1, MI_G1,  MI_Gs1, MI_A1, MI_As1, MI_B1,  MI_C2,  MI_Cs2, MI_D2, \
    MI_E,   MI_F,   MI_Fs, MI_G,   MI_Gs,  MI_A,  MI_As,  MI_B,  MI_C1,       MI_Cs1, MI_D1,  MI_Ds1, MI_E1, MI_F1,  MI_Fs1, MI_G1,  MI_Gs1, MI_A1, \
    MI_BN1, MI_C,   MI_Cs, MI_D,   MI_Ds,  MI_E,  MI_F,   MI_Fs, MI_G,        MI_Gs,  MI_A,   MI_As,  MI_B,  MI_C1,  MI_Cs1, MI_D1,  MI_Ds1, MI_E1  \
),
// | 1     | 2     | 3     | 4     | 5     | 6     | 7     | 8     | 9    |||||| 10   | 11   | 12   | 13   | 14   | 15   | 16   | 17   | 18   |
[_LH_MAJOR] = LAYOUT( \
    MI_B3,  MI_C4,  MI_D4,  MI_E4,  MI_F4,  MI_G4,  MI_A4,  MI_B4,  MI_C5,      MI_D5, MI_E5, MI_F5, MI_G5, MI_A5, MI_B5, MI_C6, MI_D6, MI_E6, \
    MI_B2,  MI_C3,  MI_D3,  MI_E3,  MI_F3,  MI_G3,  MI_A3,  MI_B3,  MI_C4,      MI_D4, MI_E4, MI_F4, MI_G4, MI_A4, MI_B4, MI_C5, MI_D5, MI_E5, \
    MI_B1,  MI_C2,  MI_D2,  MI_E2,  MI_F2,  MI_G2,  MI_A2,  MI_B2,  MI_C3,      MI_D3, MI_E3, MI_F3, MI_G3, MI_A3, MI_B3, MI_C4, MI_D4, MI_E4, \
    MI_B,   MI_C1,  MI_D1,  MI_E1,  MI_F1,  MI_G1,  MI_A1,  MI_B1,  MI_C2,      MI_D2, MI_E2, MI_F2, MI_G2, MI_A2, MI_B2, MI_C3, MI_D3, MI_E3, \
    MI_BN1, MI_C,   MI_D,   MI_E,   MI_F,   MI_G,   MI_A,   MI_B,   MI_C1,      MI_D1, MI_E1, MI_F1, MI_G1, MI_A1, MI_B1, MI_C2, MI_D2, MI_E2, \
    MI_BN2, MI_CN1, MI_DN1, MI_EN1, MI_FN1, MI_GN1, MI_AN1, MI_BN1, MI_C,       MI_D,  MI_E,  MI_F,  MI_G,  MI_A,  MI_B,  MI_C1, MI_D1, MI_E1  \
),
// | 1      | 2     | 3     | 4      | 5     | 6     | 7      | 8      | 9    |||||| 10   | 11    | 12   | 13   | 14    | 15    | 16   | 17   | 18    |
[_LH_MINOR] = LAYOUT( \
    MI_Bb3,  MI_C4,  MI_D4,  MI_Eb4,  MI_F4,  MI_G4,  MI_Ab4,  MI_Bb4,  MI_C5,      MI_D5, MI_Eb5, MI_F5, MI_G5, MI_Ab5, MI_Bb5, MI_C6, MI_D6, MI_Eb6, \
    MI_Bb2,  MI_C3,  MI_D3,  MI_Eb3,  MI_F3,  MI_G3,  MI_Ab3,  MI_Bb3,  MI_C4,      MI_D4, MI_Eb4, MI_F4, MI_G4, MI_Ab4, MI_Bb4, MI_C5, MI_D5, MI_Eb5, \
    MI_Bb1,  MI_C2,  MI_D2,  MI_Eb2,  MI_F2,  MI_G2,  MI_Ab2,  MI_Bb2,  MI_C3,      MI_D3, MI_Eb3, MI_F3, MI_G3, MI_Ab3, MI_Bb3, MI_C4, MI_D4, MI_Eb4, \
    MI_Bb,   MI_C1,  MI_D1,  MI_Eb1,  MI_F1,  MI_G1,  MI_Ab1,  MI_Bb1,  MI_C2,      MI_D2, MI_Eb2, MI_F2, MI_G2, MI_Ab2, MI_Bb2, MI_C3, MI_D3, MI_Eb3, \
    MI_BbN1, MI_C,   MI_D,   MI_Eb,   MI_F,   MI_G,   MI_Ab,   MI_Bb,   MI_C1,      MI_D1, MI_Eb1, MI_F1, MI_G1, MI_Ab1, MI_Bb1, MI_C2, MI_D2, MI_Eb2, \
    MI_BbN2, MI_CN1, MI_DN1, MI_EbN1, MI_FN1, MI_GN1, MI_AbN1, MI_BbN1, MI_C,       MI_D,  MI_Eb,  MI_F,  MI_G,  MI_Ab,  MI_Bb,  MI_C1, MI_D1, MI_Eb1  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10    | 11   | 12    | 13    | 14   | 15    | 16   | 17    | 18    |
[_RH_CHROMATIC] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_Cs2, MI_D2, MI_Ds2, MI_E2,  MI_F2, MI_Fs2, MI_G2, MI_Gs2, MI_A2,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_Gs1, MI_A1, MI_As1, MI_B1,  MI_C2, MI_Cs2, MI_D2, MI_Ds2, MI_E2,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_Ds1, MI_E1, MI_F1,  MI_Fs1, MI_G1, MI_Gs1, MI_A1, MI_As1, MI_B1,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_As,  MI_B,  MI_C1,  MI_Cs1, MI_D1, MI_Ds1, MI_E1, MI_F1,  MI_Fs1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_F,   MI_Fs, MI_G,   MI_Gs,  MI_A,  MI_As,  MI_B,  MI_C1,  MI_Cs1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C,   MI_Cs, MI_D,   MI_Ds,  MI_E,  MI_F,   MI_Fs, MI_G,   MI_Gs   \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10    | 11    | 12    | 13    | 14    | 15    | 16    | 17   | 18   |
[_RH_MAJOR] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C4,  MI_D4,  MI_E4,  MI_F4,  MI_G4,  MI_A4,  MI_B4,  MI_C5, MI_D5, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C3,  MI_D3,  MI_E3,  MI_F3,  MI_G3,  MI_A3,  MI_B3,  MI_C4, MI_D4, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C2,  MI_D2,  MI_E2,  MI_F2,  MI_G2,  MI_A2,  MI_B2,  MI_C3, MI_D3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C1,  MI_D1,  MI_E1,  MI_F1,  MI_G1,  MI_A1,  MI_B1,  MI_C2, MI_D2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C,   MI_D,   MI_E,   MI_F,   MI_G,   MI_A,   MI_B,   MI_C1, MI_D1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_CN1, MI_DN1, MI_EN1, MI_FN1, MI_GN1, MI_AN1, MI_BN1, MI_C,  MI_D   \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10    | 11    | 12     | 13    | 14    | 15     | 16     | 17   | 18   |
[_RH_MINOR] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C4,  MI_D4,  MI_Eb4,  MI_F4,  MI_G4,  MI_Ab4,  MI_Bb4,  MI_C5, MI_D5, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C3,  MI_D3,  MI_Eb3,  MI_F3,  MI_G3,  MI_Ab3,  MI_Bb3,  MI_C4, MI_D4, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C2,  MI_D2,  MI_Eb2,  MI_F2,  MI_G2,  MI_Ab2,  MI_Bb2,  MI_C3, MI_D3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C1,  MI_D1,  MI_Eb1,  MI_F1,  MI_G1,  MI_Ab1,  MI_Bb1,  MI_C2, MI_D2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C,   MI_D,   MI_Eb,   MI_F,   MI_G,   MI_Ab,   MI_Bb,   MI_C1, MI_D1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_CN1, MI_DN1, MI_EbN1, MI_FN1, MI_GN1, MI_AbN1, MI_BbN1, MI_C,  MI_D   \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RH_DRUM] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MX_CH1,  MX_CH2,  MX_CH3,  MX_CH4,  MX_CH5,  MX_CH6,  MX_CH7,  MX_CH8,  XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MX_CH9,  MX_CH10, MX_CH11, MX_CH12, MX_CH13, MX_CH14, MX_CH15, MX_CH16, XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X29,  MI_X30,  MI_X31,  MI_X32,  MI_X45,  MI_X46,  MI_X47,  MI_X48,  XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X25,  MI_X26,  MI_X27,  MI_X28,  MI_X41,  MI_X42,  MI_X43,  MI_X44,  XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X21,  MI_X22,  MI_X23,  MI_X24,  MI_X37,  MI_X38,  MI_X39,  MI_X40,  XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X17,  MI_X18,  MI_X19,  MI_X20,  MI_X33,  MI_X34,  MI_X35,  MI_X36,  XXXXXXX  \
),
// TODO: Add fancy pattern change keys, etc. to unused right columns.
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RH_PO] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X41,  MI_X42,  MI_X43,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X33,  MI_X34,  MI_X35,  MI_X36,  MI_X44,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X37,  MI_X38,  MI_X39,  MI_X40,  MI_X45,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X25,  MI_X26,  MI_X27,  MI_X28,  MI_X46,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_X29,  MI_X30,  MI_X31,  MI_X32,  MI_X47,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
),
// | 1       | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9     |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_LC_PERFORM] = LAYOUT( \
    MI_LEG,  _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CHNU, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CHND, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_BNDU, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_BNDD, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_SUST, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_LC_TRANSPOSE] = LAYOUT( \
    MI_TR0,  _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_OC2,  _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_TRSU, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_TRSD, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_OCTU, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_OCTD, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_LC_CCS] = LAYOUT( \
    MI_CCT9, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT8, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT7, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM9, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM8, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM7, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_LC_CCM] = LAYOUT( \
    MI_CCM6, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM5, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM4, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM3, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM2, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM1, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_LC_CCT] = LAYOUT( \
    MI_CCT6, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT5, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT4, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT3, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT2, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT1, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
// | 1     | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_LC_CHANNEL] = LAYOUT( \
    MI_CH6, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CH5, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CH4, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CH3, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CH2, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CH1, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RC_PERFORM] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_LEG,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CHNU, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CHND, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_BNDU, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_BNDD, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_SUST  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RC_TRANSPOSE] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_TR0,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_OC2,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_TRSU, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_TRSD, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_OCTU, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_OCTD  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RC_CCS] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT9, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT8, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT7, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM9, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM8, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM7  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RC_CCM] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM6, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM5, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM4, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM1  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RC_CCT] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT6, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT5, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT4, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT1  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RC_CHANNEL] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CH6, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CH5, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CH4, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CH3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CH2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CH1  \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_QWERTY] = LAYOUT( \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(_QWERTY), \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_GRV,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, XXXXXXX, KC_UP,   XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL, KC_LALT, KC_LGUI, KC_BSPC, KC_BSPC,      KC_SPC,  KC_SPC,  KC_ENT,  XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT      \
),
// | 1           | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_COMMAND_KEY] = LAYOUT( \
    MO(_COMMAND), _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
// | 1                | 2                | 3            | 4           | 5          | 6              | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18          |
[_COMMAND] = LAYOUT( \
    _______,           XXXXXXX,           XXXXXXX,       XXXXXXX,      XXXXXXX,     XXXXXXX,         XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    XXXXXXX,           XXXXXXX,           XXXXXXX,       XXXXXXX,      XXXXXXX,     XXXXXXX,         XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    TG(_RC_PERFORM),   TG(_RC_TRANSPOSE), TG(_RC_CCS),   TG(_RC_CCM),  TG(_RC_CCT), TG(_RC_CHANNEL), XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    TG(_RH_CHROMATIC), TG(_RH_MAJOR),     TG(_RH_MINOR), TG(_RH_DRUM), TG(_RH_PO),  XXXXXXX,         XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    DF(_LH_CHROMATIC), DF(_LH_MAJOR),     DF(_LH_MINOR), XXXXXXX,      XXXXXXX,     XXXXXXX,         XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(_QWERTY),  \
    TG(_LC_PERFORM),   TG(_LC_TRANSPOSE), TG(_LC_CCS),   TG(_LC_CCM),  TG(_LC_CCT), TG(_LC_CHANNEL), XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OSL(_CONTROL) \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_CONTROL] = LAYOUT( \
    _______, MI_CH1,  MI_CH2,  MI_CH3,  MI_CH4,  MI_CH5,  MI_CH6,  MI_CH7,  MI_CH8,       MI_CH9,  MI_CH10, MI_CH11, MI_CH12, MI_CH13, MI_CH14, MI_CH15, MI_CH16, XXXXXXX, \
    MI_TR0,  MI_VL0,  MI_VL1,  MI_VL2,  MI_VL3,  MI_VL4,  MI_VL5,  MI_VL6,  MI_VL7,       MI_VL8,  MI_VL9,  MI_VL10, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_TR0,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_OC2,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_OC2,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_AOFF, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
)
};
