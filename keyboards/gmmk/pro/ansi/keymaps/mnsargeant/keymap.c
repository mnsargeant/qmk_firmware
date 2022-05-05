#include QMK_KEYBOARD_H


#define _LAYER0 0
#define _LAYER1 1

enum custom_keycodes {
    LAYER0 = SAFE_RANGE,
    LAYER1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      PrtSc            Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)      BackSpc           PgUp
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgDn
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             Home
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       Del
//      Ct_L     Alt_L    Win_L                               SPACE                               Alt_R    FN   Ct_R         Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,           KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,           KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,           KC_DEL,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,            KC_HOME,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,    KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           KC_MPLY,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           KC_PGDN,
        _______, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET,             KC_INS,
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______, _______,          _______,           _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,  _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, KC_MEDIA_PREV_TRACK, _______, KC_MEDIA_NEXT_TRACK
    ),
};


bool is_alt_tab_active = false;

uint16_t alt_tab_timer = 0;


// ROTARY KNOB ===============
bool encoder_update_user(uint8_t index, bool clockwise) {

    uint8_t temp_mod = get_mods();

    bool is_ctrl = temp_mod & MOD_MASK_CTRL;
    bool is_shift = temp_mod & MOD_MASK_SHIFT;

    bool is_only_ctrl = is_ctrl & !is_shift;
    bool is_only_shift = is_shift & !is_ctrl;
    // bool is_shift = (temp_mod | temp_osm) & MOD_MASK_SHIFT;


    if (index == 0) {
        switch(biton32(layer_state)) {

            // fnc change media
            case 1:
                if (clockwise) {
                    tap_code16(KC_MEDIA_NEXT_TRACK);
                } 
                else {
                    tap_code16(KC_MEDIA_PREV_TRACK);
                }

                break;

            // default fall back to:
            // vol change
            default:
                if (is_only_ctrl) {
                    if (clockwise) {
                        tap_code16(KC_MEDIA_NEXT_TRACK);
                    } 
                    else {
                        tap_code16(KC_MEDIA_PREV_TRACK);
                    }
                }
                else if (is_only_shift) {
                    unregister_mods(MOD_MASK_SHIFT);
                    if (clockwise) {
                        #ifdef MOUSEKEY_ENABLE
                            tap_code(KC_MS_WH_DOWN);
                        #else
                            tap_code(KC_PGDN);
                        #endif
                    } 
                    else {
                        #ifdef MOUSEKEY_ENABLE
                            tap_code(KC_MS_WH_UP);
                        #else
                            tap_code(KC_PGUP);
                        #endif
                    }

                    set_mods(temp_mod);
                }
                else {
                    if (clockwise) {
                        tap_code(KC_VOLU);
                    } 
                    else {
                        tap_code(KC_VOLD);
                    }    
                }
        }
    }

    return true;
}


// RGB ===============
int rValue = 255;
int gValue = 255;
int bValue = 255;

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    rgb_matrix_set_color_all(rValue, gValue, bValue);

    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        RGB_MATRIX_INDICATOR_SET_COLOR(3, 255, 0, 0);

        RGB_MATRIX_INDICATOR_SET_COLOR(67, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(70, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(73, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(76, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(80, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(83, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(87, 255, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(91, 255, 0, 0);
    }
}

/* TODO :: Create custom functions to call
void rgb_set_left_side_light_static(int r, int g, int b) {
    RGB_MATRIX_INDICATOR_SET_COLOR(67, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(70, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(73, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(76, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(80, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(83, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(87, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(91, r, g, b);
}

void rgb_set_right_side_light_static(int r, int g, int b) {
    RGB_MATRIX_INDICATOR_SET_COLOR(68, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(71, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(74, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(77, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(81, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(84, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(88, r, g, b);
    RGB_MATRIX_INDICATOR_SET_COLOR(92, r, g, b);
}
*/