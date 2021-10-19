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
    // _RH_DRUM,
    // _RH_GUITAR,
    // _RH_PO,
    // _RH_SEQUENCER,
    _END_OF_RH_LAYER_GROUP,

    // Left column.
    _LC_PERFORM = _END_OF_RH_LAYER_GROUP,
    _LC_TRANSPOSE,
    _LC_CC,
    // _LC_CHANNEL_A,
    // _LC_CHANNEL_B,
    // _LC_CHANNEL_C,
    _END_OF_LC_LAYER_GROUP,

    // Right column.
    _RC_PERFORM = _END_OF_LC_LAYER_GROUP,
    _RC_TRANSPOSE,
    _RC_CC,
    // _RC_CHANNEL_A,
    // _RC_CHANNEL_B,
    // _RC_CHANNEL_C,
    _END_OF_RC_LAYER_GROUP,
    _END_OF_LAYER_GROUPS = _END_OF_RC_LAYER_GROUP,

    // Multiple columns.
    _BLANK = _END_OF_LAYER_GROUPS,

    // Other.
    _QWERTY,

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
const layer_state_t RH_BITMASK = (1 << _END_OF_RH_LAYER_GROUP) - (1 << _START_OF_LAYER_GROUPS);
const layer_state_t LC_BITMASK = (1 << _END_OF_LC_LAYER_GROUP) - (1 << _END_OF_RH_LAYER_GROUP);
const layer_state_t RC_BITMASK = (1 << _END_OF_RC_LAYER_GROUP) - (1 << _END_OF_LC_LAYER_GROUP);

// Sequencer resolution and tempo defaults.
#define _SQ_RES_INIT 6  // SQ_RES_8, twice per beat.
#define _SQ_TMP_1 80
#define _SQ_TMP_2 120
#define _SQ_TMP_3 160

enum custom_keycodes {
    LOWER_OCTAVE_KEYCODES_START = SAFE_RANGE,
    MI_As_N2 = LOWER_OCTAVE_KEYCODES_START,
    MI_Bb_N2 = MI_As_N2,
    MI_B_N2,
    MI_C_N1,
    MI_Cs_N1,
    MI_Db_N1 = MI_Cs_N1,
    MI_D_N1,
    MI_Ds_N1,
    MI_Eb_N1 = MI_Ds_N1,
    MI_E_N1,
    MI_F_N1,
    MI_Fs_N1,
    MI_Gb_N1 = MI_Fs_N1,
    MI_G_N1,
    MI_Gs_N1,
    MI_Ab_N1 = MI_Gs_N1,
    MI_A_N1,
    MI_As_N1,
    MI_Bb_N1 = MI_As_N1,
    MI_B_N1,

    HIGHER_OCTAVE_KEYCODES_START,
    MI_C_6 = HIGHER_OCTAVE_KEYCODES_START,
    MI_Cs_6,
    MI_Db_6 = MI_Cs_6,
    MI_D_6,
    MI_Ds_6,
    MI_Eb_6 = MI_Ds_6,
    MI_E_6,
    // MI_F_6,
    // MI_Fs_6,
    // MI_Gb_6 = MI_Fs_6,
    // MI_G_6,
    // MI_Gs_6,
    // MI_Ab_6 = MI_Gs_6,

    // MIDI CC codes 102-119 are undefined in the spec, so we can use them
    // as general purpose controls.
    CC_KEYCODES_START,
    // Momentary.
    MI_CCM1 = CC_KEYCODES_START,
    MI_CCM2,
    MI_CCM3,
    // Toggle.
    MI_CCT1,
    MI_CCT2,
    MI_CCT3,

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

    sequencer_config.track_notes[0] = MI_C_2;
    sequencer_config.track_notes[1] = MI_Cs_2;
    sequencer_config.track_notes[2] = MI_D_2;
    sequencer_config.track_notes[3] = MI_Ds_2;
    sequencer_config.track_notes[4] = MI_E_2;
    sequencer_config.track_notes[5] = MI_F_2;
    sequencer_config.track_notes[6] = MI_Fs_2;
    sequencer_config.track_notes[7] = MI_G_2;
}

bool toggle_cc_state[3] = { false };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t note_number = 0;
    uint8_t momentary_cc_number = 0;
    uint8_t toggle_cc_number = 0;

    switch (keycode) {
        // MIDI note extensions outside of the regular octave range.
        case MI_As_N2:
        case MI_B_N2:
        case MI_C_N1:
        case MI_Cs_N1:
        case MI_D_N1:
        case MI_Ds_N1:
        case MI_E_N1:
        case MI_F_N1:
        case MI_Fs_N1:
        case MI_G_N1:
        case MI_Gs_N1:
        case MI_A_N1:
        case MI_As_N1:
        case MI_B_N1:
            note_number = keycode + 33 - LOWER_OCTAVE_KEYCODES_START;
            break;
        case MI_C_6:
        case MI_Cs_6:
        case MI_D_6:
        case MI_Ds_6:
        case MI_E_6:
        // case MI_F_6:
        // case MI_Fs_6:
        // case MI_G_6:
        // case MI_Gs_6:
            note_number = keycode + 119 - HIGHER_OCTAVE_KEYCODES_START;
            break;
        case MI_CCM1:
        case MI_CCM2:
        case MI_CCM3:
            momentary_cc_number = keycode + 102 - CC_KEYCODES_START;
            break;
        case MI_CCT1:
        case MI_CCT2:
        case MI_CCT3:
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

    if (note_number) {
        if (record->event.pressed) {
            midi_send_noteon(&midi_device, midi_config.channel, midi_compute_note(note_number), midi_config.velocity);
        } else {
            midi_send_noteoff(&midi_device, midi_config.channel, midi_compute_note(note_number), 0);
        }
    // CC messages always send on Channel 1.
    } else if (momentary_cc_number) {
        midi_send_cc(&midi_device, 0, momentary_cc_number, record->event.pressed ? 127 : 0);
    } else if (toggle_cc_number && record->event.pressed) {
        bool toggle_state = !toggle_cc_state[toggle_cc_number - 102];
        toggle_cc_state[toggle_cc_number - 102] = toggle_state;
        midi_send_cc(&midi_device, 0, toggle_cc_number, toggle_state ? 127 : 0);
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

//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
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
//  1        2        3       4        5        6       7        8       9             10       11       12       13      14       15       16       17       18
[_LH_CHROMATIC] = LAYOUT( \
    MI_C_2,  MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2, MI_Fs_2, MI_G_2, MI_Gs_2,      MI_A_2,  MI_As_2, MI_B_2,  MI_C_3, MI_Cs_3, MI_D_3,  MI_Ds_3, MI_E_3,  MI_F_3, \
    MI_G_1,  MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2, MI_Cs_2, MI_D_2, MI_Ds_2,      MI_E_2,  MI_F_2,  MI_Fs_2, MI_G_2, MI_Gs_2, MI_A_2,  MI_As_2, MI_B_2,  MI_C_3, \
    MI_D_1,  MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1, MI_Gs_1, MI_A_1, MI_As_1,      MI_B_1,  MI_C_2,  MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2,  MI_Fs_2, MI_G_2, \
    MI_A,    MI_As,   MI_B,   MI_C_1,  MI_Cs_1, MI_D_1, MI_Ds_1, MI_E_1, MI_F_1,       MI_Fs_1, MI_G_1,  MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2,  MI_Cs_2, MI_D_2, \
    MI_E,    MI_F,    MI_Fs,  MI_G,    MI_Gs,   MI_A,   MI_As,   MI_B,   MI_C_1,       MI_Cs_1, MI_D_1,  MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1,  MI_Gs_1, MI_A_1, \
    MI_B_N1, MI_C,    MI_Cs,  MI_D,    MI_Ds,   MI_E,   MI_F,    MI_Fs,  MI_G,         MI_Gs,   MI_A,    MI_As,   MI_B,   MI_C_1,  MI_Cs_1, MI_D_1,  MI_Ds_1, MI_E_1  \
),
//  1        2        3        4        5        6        7        8        9            10      11      12      13      14      15      16      17      18
[_LH_MAJOR] = LAYOUT( \
    MI_B_3,  MI_C_4,  MI_D_4,  MI_E_4,  MI_F_4,  MI_G_4,  MI_A_4,  MI_B_4,  MI_C_5,      MI_D_5, MI_E_5, MI_F_5, MI_G_5, MI_A_5, MI_B_5, MI_C_6, MI_D_6, MI_E_6, \
    MI_B_2,  MI_C_3,  MI_D_3,  MI_E_3,  MI_F_3,  MI_G_3,  MI_A_3,  MI_B_3,  MI_C_4,      MI_D_4, MI_E_4, MI_F_4, MI_G_4, MI_A_4, MI_B_4, MI_C_5, MI_D_5, MI_E_5, \
    MI_B_1,  MI_C_2,  MI_D_2,  MI_E_2,  MI_F_2,  MI_G_2,  MI_A_2,  MI_B_2,  MI_C_3,      MI_D_3, MI_E_3, MI_F_3, MI_G_3, MI_A_3, MI_B_3, MI_C_4, MI_D_4, MI_E_4, \
    MI_B,    MI_C_1,  MI_D_1,  MI_E_1,  MI_F_1,  MI_G_1,  MI_A_1,  MI_B_1,  MI_C_2,      MI_D_2, MI_E_2, MI_F_2, MI_G_2, MI_A_2, MI_B_2, MI_C_3, MI_D_3, MI_E_3, \
    MI_B_N1, MI_C,    MI_D,    MI_E,    MI_F,    MI_G,    MI_A,    MI_B,    MI_C_1,      MI_D_1, MI_E_1, MI_F_1, MI_G_1, MI_A_1, MI_B_1, MI_C_2, MI_D_2, MI_E_2,  \
    MI_B_N2, MI_C_N1, MI_D_N1, MI_E_N1, MI_F_N1, MI_G_N1, MI_A_N1, MI_B_N1, MI_C,        MI_D,   MI_E,   MI_F,   MI_G,   MI_A,   MI_B,   MI_C_1, MI_D_1, MI_E_1  \
),
//  1         2        3        4         5        6        7         8         9            10      11       12      13      14       15       16      17      18
[_LH_MINOR] = LAYOUT( \
    MI_Bb_3,  MI_C_4,  MI_D_4,  MI_Eb_4,  MI_F_4,  MI_G_4,  MI_Ab_4,  MI_Bb_4,  MI_C_5,      MI_D_5, MI_Eb_5, MI_F_5, MI_G_5, MI_Ab_5, MI_Bb_5, MI_C_6, MI_D_6, MI_Eb_6, \
    MI_Bb_2,  MI_C_3,  MI_D_3,  MI_Eb_3,  MI_F_3,  MI_G_3,  MI_Ab_3,  MI_Bb_3,  MI_C_4,      MI_D_4, MI_Eb_4, MI_F_4, MI_G_4, MI_Ab_4, MI_Bb_4, MI_C_5, MI_D_5, MI_Eb_5, \
    MI_Bb_1,  MI_C_2,  MI_D_2,  MI_Eb_2,  MI_F_2,  MI_G_2,  MI_Ab_2,  MI_Bb_2,  MI_C_3,      MI_D_3, MI_Eb_3, MI_F_3, MI_G_3, MI_Ab_3, MI_Bb_3, MI_C_4, MI_D_4, MI_Eb_4, \
    MI_Bb,    MI_C_1,  MI_D_1,  MI_Eb_1,  MI_F_1,  MI_G_1,  MI_Ab_1,  MI_Bb_1,  MI_C_2,      MI_D_2, MI_Eb_2, MI_F_2, MI_G_2, MI_Ab_2, MI_Bb_2, MI_C_3, MI_D_3, MI_Eb_3, \
    MI_Bb_N1, MI_C,    MI_D,    MI_Eb,    MI_F,    MI_G,    MI_Ab,    MI_Bb,    MI_C_1,      MI_D_1, MI_Eb_1, MI_F_1, MI_G_1, MI_Ab_1, MI_Bb_1, MI_C_2, MI_D_2, MI_Eb_2, \
    MI_Bb_N2, MI_C_N1, MI_D_N1, MI_Eb_N1, MI_F_N1, MI_G_N1, MI_Ab_N1, MI_Bb_N1, MI_C,        MI_D,   MI_Eb,   MI_F,   MI_G,   MI_Ab,   MI_Bb,   MI_C_1, MI_D_1, MI_Eb_1  \
),
//  1        2        3        4        5        6        7        8        9             10       11      12       13       14      15       16      17       18
[_RH_CHROMATIC] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2, MI_Fs_2, MI_G_2, MI_Gs_2, MI_A_2,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2, MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1, MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_As,   MI_B,   MI_C_1,  MI_Cs_1, MI_D_1, MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_F,    MI_Fs,  MI_G,    MI_Gs,   MI_A,   MI_As,   MI_B,   MI_C_1,  MI_Cs_1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C,    MI_Cs,  MI_D,    MI_Ds,   MI_E,   MI_F,    MI_Fs,  MI_G,    MI_Gs    \
),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17      18
[_RH_MAJOR] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_4,  MI_D_4,  MI_E_4,  MI_F_4,  MI_G_4,  MI_A_4,  MI_B_4,  MI_C_5, MI_D_5, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_3,  MI_D_3,  MI_E_3,  MI_F_3,  MI_G_3,  MI_A_3,  MI_B_3,  MI_C_4, MI_D_4, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_2,  MI_D_2,  MI_E_2,  MI_F_2,  MI_G_2,  MI_A_2,  MI_B_2,  MI_C_3, MI_D_3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_1,  MI_D_1,  MI_E_1,  MI_F_1,  MI_G_1,  MI_A_1,  MI_B_1,  MI_C_2, MI_D_2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C,    MI_D,    MI_E,    MI_F,    MI_G,    MI_A,    MI_B,    MI_C_1, MI_D_1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_N1, MI_D_N1, MI_E_N1, MI_F_N1, MI_G_N1, MI_A_N1, MI_B_N1, MI_C,   MI_D    \
),
//  1        2        3        4        5        6        7        8        9             10       11       12        13       14       15        16        17      18
[_RH_MINOR] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_4,  MI_D_4,  MI_Eb_4,  MI_F_4,  MI_G_4,  MI_Ab_4,  MI_Bb_4,  MI_C_5, MI_D_5, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_3,  MI_D_3,  MI_Eb_3,  MI_F_3,  MI_G_3,  MI_Ab_3,  MI_Bb_3,  MI_C_4, MI_D_4, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_2,  MI_D_2,  MI_Eb_2,  MI_F_2,  MI_G_2,  MI_Ab_2,  MI_Bb_2,  MI_C_3, MI_D_3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_1,  MI_D_1,  MI_Eb_1,  MI_F_1,  MI_G_1,  MI_Ab_1,  MI_Bb_1,  MI_C_2, MI_D_2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C,    MI_D,    MI_Eb,    MI_F,    MI_G,    MI_Ab,    MI_Bb,    MI_C_1, MI_D_1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      MI_C_N1, MI_D_N1, MI_Eb_N1, MI_F_N1, MI_G_N1, MI_Ab_N1, MI_Bb_N1, MI_C,   MI_D    \
),
//  1         2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_LC_PERFORM] = LAYOUT( \
    MI_LEG,   _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CHU,   _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CHD,   _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_BENDU, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_BENDD, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_SUS,   _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \

),
//  1          2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_LC_TRANSPOSE] = LAYOUT( \
    MI_TRNS_0, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_OCT_2,  _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_TRNSU,  _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_TRNSD,  _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_OCTU,   _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_OCTD,   _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \

),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_LC_CC] = LAYOUT( \
    MI_CCT3, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT2, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCT1, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM3, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM2, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    MI_CCM1, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \

),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_RC_PERFORM] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_LEG,   \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CHU,   \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CHD,   \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_BENDU, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_BENDD, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_SUS    \

),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_RC_TRANSPOSE] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_TRNS_0, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_OCT_2,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_TRNSU,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_TRNSD,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_OCTU,   \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_OCTD    \

),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_RC_CC] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCT1, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM3, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM2, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, MI_CCM1  \

),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_QWERTY] = LAYOUT( \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(_QWERTY), \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_GRV,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, XXXXXXX, KC_UP,   XXXXXXX,     \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL, KC_LALT, KC_LGUI, KC_BSPC, KC_BSPC,      KC_SPC,  KC_SPC,  KC_ENT,  XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT      \
),
//  1             2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_COMMAND_KEY] = LAYOUT( \
    MO(_COMMAND), _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______,      _______, _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),
//  1                  2                  3              4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_COMMAND] = LAYOUT( \
    _______,           XXXXXXX,           XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    XXXXXXX,           XXXXXXX,           XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    TG(_RC_PERFORM),   TG(_RC_TRANSPOSE), TG(_RC_CC),    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    TG(_RH_CHROMATIC), TG(_RH_MAJOR),     TG(_RH_MINOR), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    DF(_LH_CHROMATIC), DF(_LH_MAJOR),     DF(_LH_MINOR), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(_QWERTY),  \
    TG(_LC_PERFORM),   TG(_LC_TRANSPOSE), TG(_LC_CC),    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OSL(_CONTROL) \
),
//  1          2         3         4         5         6         7         8         9              10        11        12         13       14       15       16       17       18
[_CONTROL] = LAYOUT( \
    _______,   MI_CH1,   MI_CH2,   MI_CH3,   MI_CH4,   MI_CH5,   MI_CH6,   MI_CH7,   MI_CH8,        MI_CH9,   MI_CH10,  MI_CH11,   MI_CH12, MI_CH13, MI_CH14, MI_CH15, MI_CH16, XXXXXXX, \
    MI_TRNS_0, MI_VEL_0, MI_VEL_1, MI_VEL_2, MI_VEL_3, MI_VEL_4, MI_VEL_5, MI_VEL_6, MI_VEL_7,      MI_VEL_8, MI_VEL_9, MI_VEL_10, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_TRNS_0, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,       XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_OCT_2,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,       XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_OCT_2,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,       XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_ALLOFF, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,       XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
)
};
