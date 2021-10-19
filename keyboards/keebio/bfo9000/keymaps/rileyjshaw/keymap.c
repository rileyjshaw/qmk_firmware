#include QMK_KEYBOARD_H

extern MidiDevice midi_device;

enum layers {
    // Outside column controls.
    _DEFAULT_CONTROLS,
    _CC_CONTROLS,
    _CHANNEL_CONTROLS,

    // Scale layers.
    _CHROMATIC,
    _GUITAR_CHROMATIC,
    _MAJOR_MINOR,

    // Scale layers that overlay the column controls.
    _BIG_CHROMATIC,

    // Other layers.
    _POCKET_OPERATOR,
    _SEQUENCER,
    _COMMAND,
    _SANDBOX
};

// Sequencer resolution and tempo defaults.
#define _SQ_RES_INIT 6  // SQ_RES_8, twice per beat.
#define _SQ_TMP_1 80
#define _SQ_TMP_2 120
#define _SQ_TMP_3 160

enum custom_keycodes {
    MI_B_N1 = SAFE_RANGE,
    MI_C_6,
    MI_Cs_6,
    MI_Db_6 = MI_Cs_6,
    MI_D_6,
    MI_Ds_6,
    MI_Eb_6 = MI_Ds_6,
    MI_E_6,
    MI_F_6,
    MI_Fs_6,
    MI_Gb_6 = MI_Fs_6,
    MI_G_6,
    MI_Gs_6,
    MI_Ab_6 = MI_Gs_6,
    MI_CC80,
    MI_CC81,
    MI_CC82,
    MI_CC83,

    SQ_TCLR,
    SQ_TMP1,
    SQ_TMP2,
    SQ_TMP3
};

void keyboard_post_init_user(void) {
    layer_on(_CHROMATIC);

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

bool toggle_cc_state[] = { false, false };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t note_number = 0;
    uint8_t momentary_cc_number = 0;
    uint8_t toggle_cc_number = 0;

    switch (keycode) {
        // MIDI note extensions outside of the regular octave range.
        case MI_B_N1:
            note_number = 46;
            break;
        case MI_C_6:
            note_number = 119;
            break;
        case MI_Cs_6:
            note_number = 120;
            break;
        case MI_D_6:
            note_number = 121;
            break;
        case MI_Ds_6:
            note_number = 122;
            break;
        case MI_E_6:
            note_number = 123;
            break;
        case MI_F_6:
            note_number = 124;
            break;
        case MI_Fs_6:
            note_number = 125;
            break;
        case MI_G_6:
            note_number = 126;
            break;
        case MI_Gs_6:
            note_number = 127;
            break;

        // MIDI CC “General Purpose Button 1-4”
        case MI_CC80:
            toggle_cc_number = 80;
            break;
        case MI_CC81:
            toggle_cc_number = 81;
            break;
        case MI_CC82:
            momentary_cc_number = 82;
            break;
        case MI_CC83:
            momentary_cc_number = 83;
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
        bool toggle_state = !toggle_cc_state[toggle_cc_number - 80];
        toggle_cc_state[toggle_cc_number - 80] = toggle_state;
        midi_send_cc(&midi_device, 0, toggle_cc_number, toggle_state ? 127 : 0);
    }

    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//  1             2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_DEFAULT_CONTROLS] = LAYOUT( \
    MO(_COMMAND), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_TRNSU,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_TRNSD,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_OCTU,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_OCTD,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_SUS,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
),
//  1             2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_CC_CONTROLS] = LAYOUT( \
    MO(_COMMAND), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CC83,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CC82,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CC81,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CC80,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_SUS,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
),
//  1             2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_CHANNEL_CONTROLS] = LAYOUT( \
    MO(_COMMAND), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CH4,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CH3,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CH2,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CH1,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_SUS,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
),
//  1        2        3       4        5        6       7        8       9             10       11       12       13      14       15       16       17       18
[_CHROMATIC] = LAYOUT( \
    _______, MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2, MI_Fs_2, MI_G_2, MI_Gs_2,      MI_A_2,  MI_As_2, MI_B_2,  MI_C_3, MI_Cs_3, MI_D_3,  MI_Ds_3, MI_E_3,  MI_F_3, \
    _______, MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2, MI_Cs_2, MI_D_2, MI_Ds_2,      MI_E_2,  MI_F_2,  MI_Fs_2, MI_G_2, MI_Gs_2, MI_A_2,  MI_As_2, MI_B_2,  MI_C_3, \
    _______, MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1, MI_Gs_1, MI_A_1, MI_As_1,      MI_B_1,  MI_C_2,  MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2,  MI_Fs_2, MI_G_2, \
    _______, MI_As,   MI_B,   MI_C_1,  MI_Cs_1, MI_D_1, MI_Ds_1, MI_E_1, MI_F_1,       MI_Fs_1, MI_G_1,  MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2,  MI_Cs_2, MI_D_2, \
    _______, MI_F,    MI_Fs,  MI_G,    MI_Gs,   MI_A,   MI_As,   MI_B,   MI_C_1,       MI_Cs_1, MI_D_1,  MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1,  MI_Gs_1, MI_A_1, \
    _______, MI_C,    MI_Cs,  MI_D,    MI_Ds,   MI_E,   MI_F,    MI_Fs,  MI_G,         MI_Gs,   MI_A,    MI_As,   MI_B,   MI_C_1,  MI_Cs_1, MI_D_1,  MI_Ds_1, MI_E_1  \
),
//  1        2       3        4        5        6        7       8        9             10       11       12      13       14      15       16       17       18
[_GUITAR_CHROMATIC] = LAYOUT( \
    _______, MI_E_2, MI_F_2,  MI_Fs_2, MI_G_2,  MI_Gs_2, MI_A_2, MI_As_2, MI_B_2,       MI_C_3,  MI_Cs_3, MI_D_3, MI_Ds_3, MI_E_3, MI_F_3,  MI_Fs_3, MI_G_3,  MI_Gs_3, \
    _______, MI_B_1, MI_C_2,  MI_Cs_2, MI_D_2,  MI_Ds_2, MI_E_2, MI_F_2,  MI_Fs_2,      MI_G_2,  MI_Gs_2, MI_A_2, MI_As_2, MI_B_2, MI_C_3,  MI_Cs_3, MI_D_3,  MI_Ds_3, \
    _______, MI_G_1, MI_Gs_1, MI_A_1,  MI_As_1, MI_B_1,  MI_C_2, MI_Cs_2, MI_D_2,       MI_Ds_2, MI_E_2,  MI_F_2, MI_Fs_2, MI_G_2, MI_Gs_2, MI_A_2,  MI_As_2, MI_B_2,  \
    _______, MI_D_1, MI_Ds_1, MI_E_1,  MI_F_1,  MI_Fs_1, MI_G_1, MI_Gs_1, MI_A_1,       MI_As_1, MI_B_1,  MI_C_2, MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2,  MI_Fs_2, \
    _______, MI_A,   MI_As,   MI_B,    MI_C_1,  MI_Cs_1, MI_D_1, MI_Ds_1, MI_E_1,       MI_F_1,  MI_Fs_1, MI_G_1, MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2,  MI_Cs_2,  \
    _______, MI_E,   MI_F,    MI_Fs,   MI_G,    MI_Gs,   MI_A,   MI_As,   MI_B,         MI_C_1,  MI_Cs_1, MI_D_1, MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1,  MI_Gs_1  \
),
//  1        2       3       4       5       6       7       8       9            10      11      12       13      14      15       16       17      18
[_MAJOR_MINOR] = LAYOUT( \
    _______, MI_C_5, MI_D_5, MI_E_5, MI_F_5, MI_G_5, MI_A_5, MI_B_5, MI_C_6,      MI_C_5, MI_D_5, MI_Eb_5, MI_F_5, MI_G_5, MI_Ab_5, MI_Bb_5, MI_C_6, XXXXXXX, \
    _______, MI_C_4, MI_D_4, MI_E_4, MI_F_4, MI_G_4, MI_A_4, MI_B_4, MI_C_5,      MI_C_4, MI_D_4, MI_Eb_4, MI_F_4, MI_G_4, MI_Ab_4, MI_Bb_4, MI_C_5, XXXXXXX, \
    _______, MI_C_3, MI_D_3, MI_E_3, MI_F_3, MI_G_3, MI_A_3, MI_B_3, MI_C_4,      MI_C_3, MI_D_3, MI_Eb_3, MI_F_3, MI_G_3, MI_Ab_3, MI_Bb_3, MI_C_4, XXXXXXX, \
    _______, MI_C_2, MI_D_2, MI_E_2, MI_F_2, MI_G_2, MI_A_2, MI_B_2, MI_C_3,      MI_C_2, MI_D_2, MI_Eb_2, MI_F_2, MI_G_2, MI_Ab_2, MI_Bb_2, MI_C_3, XXXXXXX, \
    _______, MI_C_1, MI_D_1, MI_E_1, MI_F_1, MI_G_1, MI_A_1, MI_B_1, MI_C_2,      MI_C_1, MI_D_1, MI_Eb_1, MI_F_1, MI_G_1, MI_Ab_1, MI_Bb_1, MI_C_2, XXXXXXX, \
    _______, MI_C,   MI_D,   MI_E,   MI_F,   MI_G,   MI_A,   MI_B,   MI_C_1,      MI_C,   MI_D,   MI_Eb,   MI_F,   MI_G,   MI_Ab,   MI_Bb,   MI_C_1, XXXXXXX  \
),
//  1        2        3       4        5        6       7        8       9             10       11       12       13      14       15       16       17       18
[_BIG_CHROMATIC] = LAYOUT( \
    _______, MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2, MI_Fs_2, MI_G_2, MI_Gs_2,      MI_A_2,  MI_As_2, MI_B_2,  MI_C_3, MI_Cs_3, MI_D_3,  MI_Ds_3, MI_E_3,  MI_F_3, \
    MI_G_1,  MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2, MI_Cs_2, MI_D_2, MI_Ds_2,      MI_E_2,  MI_F_2,  MI_Fs_2, MI_G_2, MI_Gs_2, MI_A_2,  MI_As_2, MI_B_2,  MI_C_3, \
    MI_D_1,  MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1, MI_Gs_1, MI_A_1, MI_As_1,      MI_B_1,  MI_C_2,  MI_Cs_2, MI_D_2, MI_Ds_2, MI_E_2,  MI_F_2,  MI_Fs_2, MI_G_2, \
    MI_A,    MI_As,   MI_B,   MI_C_1,  MI_Cs_1, MI_D_1, MI_Ds_1, MI_E_1, MI_F_1,       MI_Fs_1, MI_G_1,  MI_Gs_1, MI_A_1, MI_As_1, MI_B_1,  MI_C_2,  MI_Cs_2, MI_D_2, \
    MI_E,    MI_F,    MI_Fs,  MI_G,    MI_Gs,   MI_A,   MI_As,   MI_B,   MI_C_1,       MI_Cs_1, MI_D_1,  MI_Ds_1, MI_E_1, MI_F_1,  MI_Fs_1, MI_G_1,  MI_Gs_1, MI_A_1, \
    MI_B_N1, MI_C,    MI_Cs,  MI_D,    MI_Ds,   MI_E,   MI_F,    MI_Fs,  MI_G,         MI_Gs,   MI_A,    MI_As,   MI_B,   MI_C_1,  MI_Cs_1, MI_D_1,  MI_Ds_1, MI_E_1  \
),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_POCKET_OPERATOR] = LAYOUT( \
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, MI_E_2,  MI_F_2,  MI_Fs_2, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, MI_Gs_1, MI_A_1,  MI_As_1, MI_B_1,  MI_G_2,  XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, MI_C_2,  MI_Cs_2, MI_D_2,  MI_Ds_2, MI_Gs_2, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, MI_C_1,  MI_Cs_1, MI_D_1,  MI_Ds_1, MI_A_2,  XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, MI_E_1,  MI_F_1,  MI_Fs_1, MI_G_1,  MI_As_2, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
),
//  1        2         3         4         5         6         7                   8         9              10       11       12       13       14       15       16       17       18
[_SEQUENCER] = LAYOUT( \
    _______, SQ_OFF,   SQ_TCLR,  SQ_TMP1,  SQ_TMP2,  SQ_TMP3,  SQ_R(_SQ_RES_INIT), SQ_SCLR,  SQ_SALL,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    SQ_RESU, SQ_T(0),  SQ_T(1),  SQ_T(2),  SQ_T(3),  SQ_T(4),  SQ_T(5),            SQ_T(6),  SQ_T(7),       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    SQ_RESD, SQ_S(0),  SQ_S(1),  SQ_S(2),  SQ_S(3),  SQ_S(4),  SQ_S(5),            SQ_S(6),  SQ_S(7),       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    SQ_TMPU, SQ_S(8),  SQ_S(9),  SQ_S(10), SQ_S(11), SQ_S(12), SQ_S(13),           SQ_S(14), SQ_S(15),      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    SQ_TMPD, SQ_S(16), SQ_S(17), SQ_S(18), SQ_S(19), SQ_S(20), SQ_S(21),           SQ_S(22), SQ_S(23),      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    SQ_TOG,  SQ_S(24), SQ_S(25), SQ_S(26), SQ_S(27), SQ_S(28), SQ_S(29),           SQ_S(30), SQ_S(31),      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
),
//  1          2                      3                      4                      5         6         7         8         9                          10        11        12         13       14       15       16       17       18
[_COMMAND] = LAYOUT( \
    _______,   MI_CH1,                MI_CH2,                MI_CH3,                MI_CH4,   MI_CH5,   MI_CH6,   MI_CH7,   MI_CH8,                    MI_CH9,   MI_CH10,  MI_CH11,   MI_CH12, MI_CH13, MI_CH14, MI_CH15, MI_CH16, XXXXXXX,              \
    MI_TRNS_0, MI_VEL_0,              MI_VEL_1,              MI_VEL_2,              MI_VEL_3, MI_VEL_4, MI_VEL_5, MI_VEL_6, MI_VEL_7,                  MI_VEL_8, MI_VEL_9, MI_VEL_10, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,              \
    MI_TRNS_0, TO(_CHROMATIC),        TO(_GUITAR_CHROMATIC), TO(_MAJOR_MINOR),      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                   XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,              \
    MI_OCT_2,  TO(_BIG_CHROMATIC),    XXXXXXX,               XXXXXXX,               XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  TO(_POCKET_OPERATOR),      XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(_POCKET_OPERATOR), \
    MI_OCT_2,  XXXXXXX,               XXXXXXX,               XXXXXXX,               XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  TO(_SEQUENCER),            XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(_SEQUENCER),       \
    MI_ALLOFF, DF(_DEFAULT_CONTROLS), DF(_CC_CONTROLS),      DF(_CHANNEL_CONTROLS), XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  TO(_SANDBOX),              XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(_SANDBOX)          \
),
//  1        2        3        4        5        6        7        8        9             10       11       12       13       14       15       16       17       18
[_SANDBOX] = LAYOUT( \
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
)
};
