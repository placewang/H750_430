SET(SOURCES components/ui_comp_label4.c
    components/ui_comp.c
    screens/ui_set.c
    screens/ui_MAIN.c
    screens/ui_config.c
    screens/ui_mode1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_frame_724a_png.c
    images/ui_img_frame_724b_png.c
    images/ui_img_frame_728_png.c
    images/ui_img_frame_723a_png.c
    images/ui_img_whq_png.c
    fonts/ui_font_CN14.c
    fonts/ui_font_CN20.c)

add_library(ui ${SOURCES})
