#include QMK_KEYBOARD_H

extern MidiDevice midi_device;

enum layers {
    // These layers underlie the entire keyboard as interchangeable base layers. Since other
    // layers overlap these, they are referred to as the “left hand” layers.
    _LH_CHROMATIC,
    _LH_MAJOR,
    _LH_MINOR,
    _LH_GUITAR,
    // _LH_DRUM,
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
    _LC_PERFORM_CHN = _END_OF_RH_LAYER_GROUP,
    _LC_PERFORM_VOL,
    _LC_PERFORM_ATK,
    _LC_PERFORM_REL,
    _LC_TRANSPOSE,
    _LC_CCS,
    _LC_CCM, // Momentary CCs
    _LC_CCT, // Toggle CCs
    _LC_CHANNEL,
    _END_OF_LC_LAYER_GROUP,

    // Right column.
    _RC_PERFORM = _END_OF_LC_LAYER_GROUP,
    _RC_TRANSPOSE,
    _RC_CCS,
    _RC_CCM, // Momentary CCs
    _RC_CCT, // Toggle CCs
    _RC_CHANNEL,
    _RC_EXPLORE,
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

layer_state_t prev_layer_state = 0;
bool is_clearing_layer_group = false;

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
    CC_MOMENTARY_KEYCODES_END = MI_CCM9,

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

    BANK_KEYCODES_START,
    MI_BNK0 = BANK_KEYCODES_START,
    MI_BNKU,
    MI_BNKD,
    BANK_KEYCODES_END = MI_BNKD,

    PC_KEYCODES_START,
    MI_PC0 = PC_KEYCODES_START,
    MI_PCU,
    MI_PCD,
    PC_KEYCODES_END = MI_PCD,

    SEQUENCER_KEYCODES_START,
    SQ_TCLR = SEQUENCER_KEYCODES_START,
    SQ_TMP1,
    SQ_TMP2,
    SQ_TMP3,

    CLEAR_KEYCODES_START,
    LC_CLR = CLEAR_KEYCODES_START,
    RH_CLR,
    RC_CLR,
    CLEAR_KEYCODES_END = RC_CLR,

    // One-off keycodes start.
    MI_TRX,

    // Continuous CC controls.
    MI_VOLU,
    MI_VOLD,
    MI_ATKU,
    MI_ATKD,
    MI_RELU,
    MI_RELD,
};

// Global state for MIDI CC controls
uint8_t midi_volume = 16;   // MIDI CC 7
uint8_t midi_attack = 0;   // MIDI CC 73
uint8_t midi_release = 0;  // MIDI CC 72

void keyboard_post_init_user(void) {
    layer_on(_LC_PERFORM_CHN);
    layer_on(_COMMAND_KEY);

    sequencer_set_tempo(_SQ_TMP_2);
    sequencer_set_resolution(_SQ_RES_INIT);

    for (uint8_t i = 0; i < 8; ++i) {
        sequencer_config.track_notes[i] = MI_C2 + i;
    }
}

bool toggle_cc_state[16][CC_KEYCODES_END + 1 - CC_TOGGLE_KEYCODES_START] = { false };
int8_t non_transposed_channel = 7; // Send drums over MIDI Channel 8 by default.
uint16_t selected_bank = 0; // aka MIDI CC 0 (MSB) and 32 (LSB).
uint8_t selected_patch = 0; // aka the Program Change number.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t note_number = 0;
    uint8_t momentary_cc_number = 0;
    uint8_t toggle_cc_number = 0;
    uint8_t fixed_channel_number = 100;  // Invalid, so we ignore it unless its set.

    bool should_play_note = false;
    bool should_send_momentary_cc = false;
    bool should_send_toggle_cc = false;
    bool should_update_patch = false;

    switch (keycode) {
        // MIDI note extensions outside of the regular octave range.
        case LOWER_OCTAVE_KEYCODES_START ... LOWER_OCTAVE_KEYCODES_END:
            note_number = keycode - LOWER_OCTAVE_KEYCODES_END + MIDI_TONE_MIN - 1;
            should_play_note = true;
            break;
        case HIGHER_OCTAVE_KEYCODES_START ... HIGHER_OCTAVE_KEYCODES_END:
            note_number = keycode - HIGHER_OCTAVE_KEYCODES_START + MIDI_TONE_MAX + 1;
            should_play_note = true;
            break;
        case NON_TRANSPOSED_CHANNEL_KEYCODES_START ... NON_TRANSPOSED_CHANNEL_KEYCODES_END:
            non_transposed_channel = keycode - NON_TRANSPOSED_CHANNEL_KEYCODES_START;
            break;
        case NON_TRANSPOSED_KEYCODES_START ... NON_TRANSPOSED_KEYCODES_END:
            note_number = keycode + 98 - NON_TRANSPOSED_KEYCODES_START - 12 * midi_config.octave - midi_config.transpose;
            fixed_channel_number = non_transposed_channel;
            should_play_note = true;
            break;
        case CC_KEYCODES_START ... CC_MOMENTARY_KEYCODES_END:
            if (record->event.pressed) {
                momentary_cc_number = keycode + 102 - CC_KEYCODES_START;
                should_send_momentary_cc = true;
            }
            break;
        case CC_TOGGLE_KEYCODES_START ... CC_KEYCODES_END:
            if (record->event.pressed) {
                toggle_cc_number = keycode + 102 - CC_KEYCODES_START;
                should_send_toggle_cc = true;
            }
            break;
        case BANK_KEYCODES_START ... BANK_KEYCODES_END:
            if (record->event.pressed) {
                should_update_patch = true;
                if (keycode == MI_BNK0 && selected_bank != 0) {
                    selected_bank = 0;
                } else if (keycode == MI_BNKU && selected_bank < 16383) {
                    selected_bank++;
                } else if (keycode == MI_BNKD && selected_bank > 0) {
                    selected_bank--;
                } else {
                    should_update_patch = false;
                }
            }
            break;
        case PC_KEYCODES_START ... PC_KEYCODES_END:
            if (record->event.pressed) {
                should_update_patch = true;
                if (keycode == MI_PC0 && selected_patch != 0) {
                    selected_patch = 0;
                } else if (keycode == MI_PCU && selected_patch < 127) {
                    selected_patch++;
                } else if (keycode == MI_PCD && selected_patch > 0) {
                    selected_patch--;
                } else {
                    should_update_patch = false;
                }
            }
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
            break;
        // Clear layer groups.
        case CLEAR_KEYCODES_START ... CLEAR_KEYCODES_END:
            if (record->event.pressed) {
                is_clearing_layer_group = true;
                if (keycode == LC_CLR) {
                    layer_and(~LC_BITMASK);
                }
                else if (keycode == RH_CLR) {
                    layer_and(~RH_BITMASK);
                }
                else if (keycode == RC_CLR) {
                    layer_and(~RC_BITMASK);
                }
            }
            break;
        case MI_TRX:
            if (record->event.pressed) {
                midi_config.octave = QK_MIDI_OCTAVE_2 - MIDI_OCTAVE_MIN;
                midi_config.transpose = 0;
            }
            break;
        case MI_VOLU:
            if (record->event.pressed && midi_volume < 127) {
                midi_volume += 8;
                midi_send_cc(&midi_device, midi_config.channel, 7, midi_volume);
            }
            break;
        case MI_VOLD:
            if (record->event.pressed && midi_volume > 0) {
                midi_volume -= 8;
                midi_send_cc(&midi_device, midi_config.channel, 7, midi_volume);
            }
            break;
        case MI_ATKU:
            if (record->event.pressed && midi_attack < 127) {
                midi_attack += 8;
                midi_send_cc(&midi_device, midi_config.channel, 73, midi_attack);
            }
            break;
        case MI_ATKD:
            if (record->event.pressed && midi_attack > 0) {
                midi_attack -= 8;
                midi_send_cc(&midi_device, midi_config.channel, 73, midi_attack);
            }
            break;
        case MI_RELU:
            if (record->event.pressed && midi_release < 127) {
                midi_release += 8;
                midi_send_cc(&midi_device, midi_config.channel, 72, midi_release);
            }
            break;
        case MI_RELD:
            if (record->event.pressed && midi_release > 0) {
                midi_release -= 8;
                midi_send_cc(&midi_device, midi_config.channel, 72, midi_release);
            }
            break;
    /*Amen*/break;
        default:
            return true;
    }

    uint8_t midi_channel = fixed_channel_number < 16 ? fixed_channel_number : midi_config.channel;

    if (should_play_note) {
        if (record->event.pressed) {
            midi_send_noteon(&midi_device, midi_channel, midi_compute_note(note_number), midi_config.velocity);
        } else {
            midi_send_noteoff(&midi_device, midi_channel, midi_compute_note(note_number), 0);
        }
    } else if (should_send_momentary_cc) {
        midi_send_cc(&midi_device, midi_channel, momentary_cc_number, record->event.pressed ? 127 : 0);
    } else if (should_send_toggle_cc) {
        bool toggle_state = !toggle_cc_state[midi_channel][toggle_cc_number - 102];
        toggle_cc_state[midi_channel][toggle_cc_number - 102] = toggle_state;
        midi_send_cc(&midi_device, midi_channel, toggle_cc_number, toggle_state ? 127 : 0);
    } else if (should_update_patch) {
        midi_send_cc(&midi_device, midi_channel, 0, (selected_bank & 0xFF00) >> 8);
        midi_send_cc(&midi_device, midi_channel, 32, selected_bank & 0x00FF);
        midi_send_programchange(&midi_device, midi_channel, selected_patch);
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Create a bitmask of changed layers. A 1 bit indicates change in either direction.
    layer_state_t changed_layer_bitmask = state ^ prev_layer_state;
    uint8_t changed_layer = get_highest_layer(changed_layer_bitmask);

    // If a layer is out of range or is being cleared, don’t do anything fancy.
    // NOTE: When selecting layers from the command menu, the chosen layer
    //       should always be turned on. QMK only has a “toggle layer” function
    //       (TG), and is lacking a persistent “activate layer” function. So as
    //       a hack, unless we’re explicitly clearing a layer group, we set any
    //       toggled layer to be on.
    if (changed_layer >= _START_OF_LAYER_GROUPS && changed_layer <= _END_OF_LAYER_GROUPS && (!is_clearing_layer_group || IS_LAYER_ON_STATE(state, changed_layer))) {
        // Clear all layers in the layer group.
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

    is_clearing_layer_group = false;
    prev_layer_state = state;
    return state;
}

/* To save flash space, the keymap is stored in compressed form and expanded on
 * the fly by the `keymap_key_to_keycode()` override at the bottom of this
 * file. Storing all 29 layers as full LAYOUT() grids would cost 6264 bytes of
 * PROGMEM (29 layers × 108 keys × 2 bytes), most of it `_______` padding or
 * arithmetically predictable MIDI notes.
 *
 *   - Note layers (_LH_* and _RH_CHROMATIC/_RH_MAJOR/_RH_MINOR) are generated
 *     from 6 per-row base semitones: each row ascends left-to-right either
 *     chromatically (+1 semitone per column) or along a repeating 7-note
 *     scale (+1 octave every 7 columns).
 *   - _LC_* and _RC_* layers define 6 keys in their outermost column and are
 *     transparent everywhere else; identical columns are shared between the
 *     left- and right-hand variants.
 *   - _RH_DRUM and _RH_PO store only their right-hand 6×9 grid.
 *   - _QWERTY, _COMMAND, and _CONTROL remain full LAYOUT() grids in
 *     `keymaps[]`, indexed by the RAW_* enum rather than by layer number.
 *
 * The matrix is split: matrix rows 0-5 are the left half and rows 6-11 the
 * right half, each 9 columns wide reading left to right.
 */

// Semitone offsets are relative to MI_C (the lowest standard MIDI keycode).
// Values below 0 or above the MI_C..MI_B5 range map onto the custom
// lower/higher octave extension keycodes defined above.
static uint16_t midi_note_keycode(int16_t semitone) {
    if (semitone < 0) {
        return LOWER_OCTAVE_KEYCODES_END + 1 + semitone;
    }
    if (semitone >= 12 * MIDI_TONE_KEYCODE_OCTAVES) {
        return HIGHER_OCTAVE_KEYCODES_START + semitone - 12 * MIDI_TONE_KEYCODE_OCTAVES;
    }
    return MI_C + semitone;
}

// Semitone offset of each of the 7 scale degrees laid out along a row,
// relative to the row’s base note.
static const int8_t scale_major_from_b[7] PROGMEM  = {0, 1, 3, 5, 6, 8, 10};
static const int8_t scale_minor_from_bb[7] PROGMEM = {0, 2, 4, 5, 7, 9, 10};
static const int8_t scale_major_from_c[7] PROGMEM  = {0, 2, 4, 5, 7, 9, 11};
static const int8_t scale_minor_from_c[7] PROGMEM  = {0, 2, 3, 5, 7, 8, 10};

typedef struct {
    int8_t row_base[6];    // Semitone of each row’s leftmost key, top row first.
    const int8_t *degrees; // 7-note scale along the row, or NULL for chromatic.
} note_grid_t;

// Indexed directly by layer (_LH_CHROMATIC.._LH_GUITAR, _RH_CHROMATIC.._RH_MINOR).
static const note_grid_t note_grids[] PROGMEM = {
    [_LH_CHROMATIC] = {{24, 19, 14, 9, 4, -1}, NULL},
    [_LH_MAJOR]     = {{47, 35, 23, 11, -1, -13}, scale_major_from_b},
    [_LH_MINOR]     = {{46, 34, 22, 10, -2, -14}, scale_minor_from_bb},
    [_LH_GUITAR]    = {{23, 18, 14, 9, 4, -1}, NULL},
    [_RH_CHROMATIC] = {{25, 20, 15, 10, 5, 0}, NULL},
    [_RH_MAJOR]     = {{48, 36, 24, 12, 0, -12}, scale_major_from_c},
    [_RH_MINOR]     = {{48, 36, 24, 12, 0, -12}, scale_minor_from_c},
};

static uint16_t note_grid_keycode(uint8_t layer, uint8_t vrow, uint8_t vcol) {
    const note_grid_t *grid = &note_grids[layer];
    int16_t semitone = (int8_t)pgm_read_byte(&grid->row_base[vrow]);
    const int8_t *degrees = (const int8_t *)pgm_read_ptr(&grid->degrees);
    if (degrees == NULL) {
        semitone += vcol;
    } else {
        semitone += (int8_t)pgm_read_byte(&degrees[vcol % 7]) + 12 * (vcol / 7);
    }
    return midi_note_keycode(semitone);
}

// The 6 keys in the outermost column of each _LC_*/_RC_* layer, top row first.
// The first nine entries match the _LC_* layer order so they can be indexed by
// `layer - _LC_PERFORM_CHN`; _RC_* layers remap through rc_column_ids below.
enum column_id { COL_PERFORM_CHN, COL_PERFORM_VOL, COL_PERFORM_ATK, COL_PERFORM_REL, COL_TRANSPOSE_L, COL_CCS, COL_CCM, COL_CCT, COL_CHANNEL, COL_TRANSPOSE_R, COL_EXPLORE };

static const uint16_t column_layer_keys[][6] PROGMEM = {
    [COL_PERFORM_CHN] = {MI_LEG,  MI_CHNU, MI_CHND, MI_BNDU, MI_BNDD, MI_SUST},
    [COL_PERFORM_VOL] = {MI_LEG,  MI_VOLU, MI_VOLD, MI_BNDU, MI_BNDD, MI_SUST},
    [COL_PERFORM_ATK] = {MI_LEG,  MI_ATKU, MI_ATKD, MI_BNDU, MI_BNDD, MI_SUST},
    [COL_PERFORM_REL] = {MI_LEG,  MI_RELU, MI_RELD, MI_BNDU, MI_BNDD, MI_SUST},
    [COL_TRANSPOSE_L] = {MI_TR0,  MI_TRX,  MI_TRSU, MI_TRSD, MI_OCTU, MI_OCTD},
    [COL_CCS]         = {MI_CCT9, MI_CCT8, MI_CCT7, MI_CCM9, MI_CCM8, MI_CCM7},
    [COL_CCM]         = {MI_CCM6, MI_CCM5, MI_CCM4, MI_CCM3, MI_CCM2, MI_CCM1},
    [COL_CCT]         = {MI_CCT6, MI_CCT5, MI_CCT4, MI_CCT3, MI_CCT2, MI_CCT1},
    [COL_CHANNEL]     = {MI_CH6,  MI_CH5,  MI_CH4,  MI_CH3,  MI_CH2,  MI_CH1},
    [COL_TRANSPOSE_R] = {MI_TR0,  MI_OC2,  MI_TRSU, MI_TRSD, MI_OCTU, MI_OCTD},
    [COL_EXPLORE]     = {MI_BNK0, MI_PC0,  MI_BNKU, MI_BNKD, MI_PCU,  MI_PCD},
};

static const uint8_t rc_column_ids[] PROGMEM = {COL_PERFORM_CHN, COL_TRANSPOSE_R, COL_CCS, COL_CCM, COL_CCT, COL_CHANNEL, COL_EXPLORE};

// Right-hand-only layers that don’t follow a formula, stored as just their
// 6×9 right half. Indexed by `layer - _RH_DRUM`.
static const uint16_t rh_grids[][6][9] PROGMEM = {
// | 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RH_DRUM - _RH_DRUM] = {
    {MX_CH1,  MX_CH2,  MX_CH3,  MX_CH4,  MX_CH5,  MX_CH6,  MX_CH7,  MX_CH8,  XXXXXXX},
    {MX_CH9,  MX_CH10, MX_CH11, MX_CH12, MX_CH13, MX_CH14, MX_CH15, MX_CH16, XXXXXXX},
    {MI_X29,  MI_X30,  MI_X31,  MI_X32,  MI_X45,  MI_X46,  MI_X47,  MI_X48,  XXXXXXX},
    {MI_X25,  MI_X26,  MI_X27,  MI_X28,  MI_X41,  MI_X42,  MI_X43,  MI_X44,  XXXXXXX},
    {MI_X21,  MI_X22,  MI_X23,  MI_X24,  MI_X37,  MI_X38,  MI_X39,  MI_X40,  XXXXXXX},
    {MI_X17,  MI_X18,  MI_X19,  MI_X20,  MI_X33,  MI_X34,  MI_X35,  MI_X36,  XXXXXXX},
},
// TODO: Add fancy pattern change keys, etc. to unused right columns.
// | 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[_RH_PO - _RH_DRUM] = {
    {XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
    {MI_X41,  MI_X42,  MI_X43,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
    {MI_X33,  MI_X34,  MI_X35,  MI_X36,  MI_X44,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
    {MI_X37,  MI_X38,  MI_X39,  MI_X40,  MI_X45,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
    {MI_X25,  MI_X26,  MI_X27,  MI_X28,  MI_X46,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
    {MI_X29,  MI_X30,  MI_X31,  MI_X32,  MI_X47,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
},
};

// Only the layers that can’t be compressed live in the raw keymaps[] array.
// NOTE: These are indexed by RAW_* position, not by layer number; the
// dispatcher below translates.
enum raw_layers { RAW_QWERTY, RAW_COMMAND, RAW_CONTROL };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// | 1          | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18         |
[RAW_QWERTY] = LAYOUT( \
    TG(_QWERTY), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(_QWERTY), \
    XXXXXXX,     XXXXXXX, XXXXXXX, KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX,     XXXXXXX, XXXXXXX, KC_GRV,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX,     XXXXXXX, XXXXXXX, KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, XXXXXXX, XXXXXXX, XXXXXXX,     \
    XXXXXXX,     XXXXXXX, XXXXXXX, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, XXXXXXX, KC_UP,   XXXXXXX,     \
    XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL, KC_LALT, KC_LGUI, KC_BSPC, KC_BSPC,      KC_SPC,  KC_SPC,  KC_ENT,  XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT      \
),
// | 1                   | 2                  | 3                | 4            | 5             | 6          | 7              | 8              | 9     |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18          |
[RAW_COMMAND] = LAYOUT( \
    _______,             XXXXXXX,             XXXXXXX,           XXXXXXX,       XXXXXXX,        XXXXXXX,     XXXXXXX,         XXXXXXX,         XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(_QWERTY),  \
    TG(_LC_PERFORM_CHN), XXXXXXX,             XXXXXXX,           XXXXXXX,       XXXXXXX,        XXXXXXX,     XXXXXXX,         XXXXXXX,         XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    TG(_LC_PERFORM_VOL), TG(_RC_PERFORM),     TG(_RC_TRANSPOSE), TG(_RC_CCS),   TG(_RC_CCM),    TG(_RC_CCT), TG(_RC_CHANNEL), TG(_RC_EXPLORE), RC_CLR,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    TG(_LC_PERFORM_ATK), TG(_RH_CHROMATIC),   TG(_RH_MAJOR),     TG(_RH_MINOR), TG(_RH_DRUM),   TG(_RH_PO),  XXXXXXX,         XXXXXXX,         RH_CLR,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    TG(_LC_PERFORM_REL), DF(_LH_CHROMATIC),   DF(_LH_MAJOR),     DF(_LH_MINOR), DF(_LH_GUITAR), XXXXXXX,     XXXXXXX,         XXXXXXX,         XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      \
    XXXXXXX,             TG(_LC_PERFORM_CHN), TG(_LC_TRANSPOSE), TG(_LC_CCS),   TG(_LC_CCM),    TG(_LC_CCT), TG(_LC_CHANNEL), XXXXXXX,         LC_CLR,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OSL(_CONTROL) \
),
// | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      |||||| 10     | 11     | 12     | 13     | 14     | 15     | 16     | 17     | 18     |
[RAW_CONTROL] = LAYOUT( \
    _______, MI_CH1,  MI_CH2,  MI_CH3,  MI_CH4,  MI_CH5,  MI_CH6,  MI_CH7,  MI_CH8,       MI_CH9,  MI_CH10, MI_CH11, MI_CH12, MI_CH13, MI_CH14, MI_CH15, MI_CH16, XXXXXXX, \
    MI_PC0,  MI_VL0,  MI_VL1,  MI_VL2,  MI_VL3,  MI_VL4,  MI_VL5,  MI_VL6,  MI_VL7,       MI_VL8,  MI_VL9,  MI_VL10, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_CH1,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_OC2,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_TR0,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    MI_AOFF, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
)
};

// Expands the compressed layer data above. Overrides the weak default in
// quantum/keymap_common.c, which reads keymaps[layer] directly.
uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
    if (key.row >= MATRIX_ROWS || key.col >= MATRIX_COLS) {
        return KC_NO;
    }

    const bool right_half = key.row >= MATRIX_ROWS / 2;
    const uint8_t vrow = right_half ? key.row - MATRIX_ROWS / 2 : key.row;

    switch (layer) {
        case _LH_CHROMATIC ... _LH_GUITAR:
            return note_grid_keycode(layer, vrow, right_half ? key.col + MATRIX_COLS : key.col);
        case _RH_CHROMATIC ... _RH_MINOR:
            return right_half ? note_grid_keycode(layer, vrow, key.col) : KC_TRNS;
        case _RH_DRUM:
        case _RH_PO:
            return right_half ? pgm_read_word(&rh_grids[layer - _RH_DRUM][vrow][key.col]) : KC_TRNS;
        case _LC_PERFORM_CHN ... _LC_CHANNEL:
            if (right_half || key.col != 0) {
                return KC_TRNS;
            }
            return pgm_read_word(&column_layer_keys[layer - _LC_PERFORM_CHN][vrow]);
        case _RC_PERFORM ... _RC_EXPLORE:
            if (!right_half || key.col != MATRIX_COLS - 1) {
                return KC_TRNS;
            }
            return pgm_read_word(&column_layer_keys[pgm_read_byte(&rc_column_ids[layer - _RC_PERFORM])][vrow]);
        case _COMMAND_KEY:
            return (key.row == 0 && key.col == 0) ? MO(_COMMAND) : KC_TRNS;
        case _QWERTY:
            return pgm_read_word(&keymaps[RAW_QWERTY][key.row][key.col]);
        case _COMMAND:
            return pgm_read_word(&keymaps[RAW_COMMAND][key.row][key.col]);
        case _CONTROL:
            return pgm_read_word(&keymaps[RAW_CONTROL][key.row][key.col]);
    }
    return KC_TRNS;
}
