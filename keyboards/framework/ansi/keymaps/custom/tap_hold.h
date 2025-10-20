#pragma once

#define QK_TAP_DANCE_GET_INDEX(kc) ((kc)&0xFF)

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

enum {
    TD_1 = 1,
    TD_2,
    TD_3,
    TD_4,
    TD_5,
    TD_6,
    TD_7,
    TD_8,
    TD_9,
    TD_10,
    TD_11,
    TD_12,
};

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold)                                        \
    {                                                                               \
        .fn        = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, \
        .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}),               \
    }

tap_dance_action_t tap_dance_actions[] = {
    [TD_1] = ACTION_TAP_DANCE_TAP_HOLD(KC_MUTE, KC_F1),
    [TD_2] = ACTION_TAP_DANCE_TAP_HOLD(KC_VOLD, KC_F2),
    [TD_3] = ACTION_TAP_DANCE_TAP_HOLD(KC_VOLU, KC_F3),
    [TD_4] = ACTION_TAP_DANCE_TAP_HOLD(KC_MPRV, KC_F4),
    [TD_5] = ACTION_TAP_DANCE_TAP_HOLD(KC_MPLY, KC_F5),
    [TD_6] = ACTION_TAP_DANCE_TAP_HOLD(KC_MNXT, KC_F6),
    [TD_7] = ACTION_TAP_DANCE_TAP_HOLD(KC_BRID, KC_F7),
    [TD_8] = ACTION_TAP_DANCE_TAP_HOLD(KC_BRIU, KC_F8),
    [TD_9] = ACTION_TAP_DANCE_TAP_HOLD(KC_SCRN, KC_F9),
    [TD_10] = ACTION_TAP_DANCE_TAP_HOLD(KC_AIRP, KC_F10),
    [TD_11] = ACTION_TAP_DANCE_TAP_HOLD(KC_PSCR, KC_F11),
    [TD_12] = ACTION_TAP_DANCE_TAP_HOLD(KC_MSEL, KC_F12),
};

bool handle_tapdance(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(TD_1):
        case TD(TD_2):
        case TD(TD_3):
        case TD(TD_4):
        case TD(TD_5):
        case TD(TD_6):
        case TD(TD_7):
        case TD(TD_8):
        case TD(TD_9):
        case TD(TD_10):
        case TD(TD_11):
        case TD(TD_12):
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
                return true;
            }
    }
    return false;
}
