/*
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  Xiegu X6100 LVGL GUI
 *
 *  Copyright (c) 2022 Belousov Oleg aka R1CBU
 */

#include "styles.h"

lv_style_t  background_style;
lv_style_t  spectrum_style;
lv_style_t  freq_style;
lv_style_t  freq_main_style;
lv_style_t  waterfall_style;
lv_style_t  btn_style;
lv_style_t  msg_style;
lv_style_t  clock_style;

extern lv_font_t    eco_sans_14;
extern lv_font_t    eco_sans_16;
extern lv_font_t    eco_sans_32;
extern lv_font_t    eco_sans_36;
extern lv_font_t    eco_sans_38;

static const uint16_t radius = 25;

void styles_init() {
    lv_style_init(&background_style);
    lv_style_set_bg_color(&background_style, lv_color_hex(0x004080));

    lv_style_init(&spectrum_style);
    lv_style_set_bg_color(&spectrum_style, lv_color_hex(0x000000));
    lv_style_set_border_color(&spectrum_style, lv_color_hex(0xAAAAAA));
    lv_style_set_border_width(&spectrum_style, 2);
    lv_style_set_radius(&spectrum_style, radius);
    lv_style_set_clip_corner(&spectrum_style, true);
    lv_style_set_width(&spectrum_style, 800-10);
    lv_style_set_x(&spectrum_style, 5);

    lv_style_init(&freq_style);
    lv_style_set_text_color(&freq_style, lv_color_white());
    lv_style_set_text_font(&freq_style, &eco_sans_16);
    lv_style_set_pad_ver(&freq_style, 10);

    lv_style_init(&freq_main_style);
    lv_style_set_text_color(&freq_main_style, lv_color_white());
    lv_style_set_text_font(&freq_main_style, &eco_sans_36);
    lv_style_set_pad_ver(&freq_main_style, 1);

    lv_style_init(&waterfall_style);
    lv_style_set_bg_color(&waterfall_style, lv_color_hex(0x000000));
    lv_style_set_border_color(&waterfall_style, lv_color_hex(0xAAAAAA));
    lv_style_set_border_width(&waterfall_style, 2);
    lv_style_set_radius(&waterfall_style, radius);
    lv_style_set_clip_corner(&waterfall_style, true);
    lv_style_set_width(&waterfall_style, 800-10);
    lv_style_set_x(&waterfall_style, 5);

    lv_style_init(&btn_style);
    lv_style_set_bg_opa(&btn_style, LV_OPA_50);
    lv_style_set_bg_color(&btn_style, lv_color_hex(0x004080));
    lv_style_set_border_opa(&btn_style, LV_OPA_50);
    lv_style_set_border_color(&btn_style, lv_color_hex(0x000000));
    lv_style_set_border_width(&btn_style, 2);
    lv_style_set_text_font(&btn_style, &eco_sans_16);
    lv_style_set_radius(&btn_style, radius);

    lv_style_init(&msg_style);
    lv_style_set_bg_opa(&msg_style, LV_OPA_50);
    lv_style_set_bg_color(&msg_style, lv_color_hex(0x004080));
    lv_style_set_text_color(&msg_style, lv_color_white());
    lv_style_set_text_font(&msg_style, &eco_sans_38);
    lv_style_set_border_opa(&msg_style, LV_OPA_50);
    lv_style_set_border_color(&msg_style, lv_color_hex(0x000000));
    lv_style_set_border_width(&msg_style, 2);
    lv_style_set_radius(&msg_style, radius);
    lv_style_set_size(&msg_style, 600, 60);
    lv_style_set_x(&msg_style, 800 / 2 - (600 / 2));
    lv_style_set_y(&msg_style, 260);
    lv_style_set_pad_ver(&msg_style, 10);

    lv_style_init(&clock_style);
    lv_style_set_bg_opa(&clock_style, LV_OPA_50);
    lv_style_set_bg_color(&clock_style, lv_color_hex(0x004080));
    lv_style_set_border_color(&clock_style, lv_color_hex(0x000000));
    lv_style_set_border_width(&clock_style, 2);
    lv_style_set_radius(&clock_style, radius);
    lv_style_set_clip_corner(&clock_style, true);
    lv_style_set_width(&clock_style, 200);
    lv_style_set_x(&clock_style, 800 - 200);
    lv_style_set_text_font(&clock_style, &eco_sans_38);
    lv_style_set_text_color(&clock_style, lv_color_white());
}
