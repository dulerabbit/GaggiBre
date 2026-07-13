#include <Arduino.h>
#include <cmath>
#include <display/main.h>
#include <display/plugins/BLEScalePlugin.h>
#include <display/ui/default/GeneratedUI.h>
#include <display/ui/default/NativeManualBrew.h>
#include <display/ui/default/shared/ManualBrewSession.h>
#include <display/ui/default/shared/ProcessControl.h>
#if !GAGGIMATE_HAS_NATIVE_MANUAL_BREW
#include <display/ui/default/manual/ManualBrewScreen.h>
#endif

namespace {

void applyClickArea(lv_obj_t *object, lv_coord_t size) {
    if (object) {
        lv_obj_set_ext_click_area(object, size);
    }
}

void suppressMeterTicks(lv_obj_t *object) {
    if (!object) {
        return;
    }
    const uint32_t count = lv_obj_get_child_cnt(object);
    for (uint32_t i = 0; i < count; ++i) {
        lv_obj_t *child = lv_obj_get_child(object, i);
        if (lv_obj_check_type(child, &lv_meter_class)) {
            auto *meter = reinterpret_cast<lv_meter_t *>(child);
            auto *scale = static_cast<lv_meter_scale_t *>(_lv_ll_get_head(&meter->scale_ll));
            if (scale && scale->tick_cnt > 0) {
                scale->tick_major_nth = scale->tick_cnt;
                scale->tick_cnt = 0;
            }
        }
        suppressMeterTicks(child);
    }
}

} // namespace

extern "C" {

void action_on_wakeup(lv_event_t *) {
    if (controller.isUpdating() || controller.isErrorState() || controller.isAutotuning() ||
        !controller.getClientController()->isConnected()) {
        return;
    }
    controller.getUI()->changeScreen(SCREEN_ID_BREW_SCREEN);
    ProcessControl::deactivate(controller);
    controller.setMode(MODE_BREW);
}

void action_on_load_started(lv_event_t *) {}

void action_on_menu_click(lv_event_t *) {
    if (controller.getMode() == MODE_MANUAL) {
        ManualBrewSession::restoreSelectedProfile(&controller);
    }
    ProcessControl::deactivate(controller);
    controller.getUI()->changeScreen(SCREEN_ID_MENU_SCREEN_NEW);
}

void action_on_brew_screen(lv_event_t *) {
    if (controller.getMode() == MODE_MANUAL) {
        ManualBrewSession::restoreSelectedProfile(&controller);
    }
    controller.getUI()->changeScreen(SCREEN_ID_BREW_SCREEN);
    ProcessControl::deactivate(controller);
    controller.setMode(MODE_BREW);
}

void action_on_steam_screen(lv_event_t *) {
    controller.getUI()->changeScreen(SCREEN_ID_STEAM_SCREEN);
    controller.setMode(MODE_STEAM);
    ProcessControl::deactivate(controller);
}

void action_on_water_screen(lv_event_t *) {
    controller.getUI()->changeScreen(SCREEN_ID_WATER_SCREEN);
    controller.setMode(MODE_WATER);
    ProcessControl::deactivate(controller);
}

void action_on_grind_screen(lv_event_t *) {
    const int secondaryAction = controller.getSettings().getSecondaryAction();
    if (secondaryAction == SECONDARY_ACTION_NONE) {
        return;
    }
    if (secondaryAction == SECONDARY_ACTION_GRIND) {
        controller.getUI()->changeScreen(SCREEN_ID_GRIND_SCREEN);
        controller.setMode(MODE_GRIND);
        ProcessControl::deactivate(controller);
        return;
    }
    ManualBrewSession::backupSelectedProfile(&controller);
    ProcessControl::deactivate(controller);
    controller.setMode(MODE_MANUAL);
    controller.setManualPressureTarget(0.0f);
    controller.getUI()->showManualBrew();
}

void action_on_brew_start(lv_event_t *) { ProcessControl::toggleActive(controller); }
void action_on_flush(lv_event_t *) { controller.onFlush(); }

void action_on_volumetric_hold(lv_event_t *) {
    controller.getClientController()->tare();
    BLEScales.tare();
}

void action_on_profile_select(lv_event_t *) { controller.getUI()->onProfileSwitch(); }
void action_on_profile_settings(lv_event_t *) { controller.getUI()->changeBrewScreenMode(BrewScreenState::Settings); }
void action_on_brew_temp_lower(lv_event_t *) {
    controller.getUI()->markProfileDirty();
    controller.lowerTemp();
}
void action_on_brew_temp_raise(lv_event_t *) {
    controller.getUI()->markProfileDirty();
    controller.raiseTemp();
}
void action_on_brew_time_raise(lv_event_t *) {
    controller.getUI()->markProfileDirty();
    controller.raiseBrewTarget();
}
void action_on_brew_time_lower(lv_event_t *) {
    controller.getUI()->markProfileDirty();
    controller.lowerBrewTarget();
}
void action_on_volumetric_delete(lv_event_t *) { controller.getUI()->onVolumetricDelete(); }
void action_on_profile_accept(lv_event_t *) { controller.getUI()->changeBrewScreenMode(BrewScreenState::Brew); }
void action_on_profile_save(lv_event_t *) {
    controller.onProfileSave();
    controller.getUI()->markProfileClean();
    controller.getUI()->changeBrewScreenMode(BrewScreenState::Brew);
}
void action_on_profile_save_as_new(lv_event_t *) {
    controller.onProfileSaveAsNew();
    controller.getUI()->markProfileClean();
    controller.getUI()->changeBrewScreenMode(BrewScreenState::Brew);
}

void action_on_meter_draw(lv_event_t *event) {
    lv_obj_t *object = lv_event_get_target(event);
    if (!lv_obj_check_type(object, &lv_meter_class)) {
        return;
    }
    auto *meter = reinterpret_cast<lv_meter_t *>(object);
    auto *scale = static_cast<lv_meter_scale_t *>(_lv_ll_get_head(&meter->scale_ll));
    lv_draw_ctx_t *drawContext = lv_event_get_draw_ctx(event);
    const uint16_t tickCount = scale ? scale->tick_major_nth : 0;
    if (!scale || !drawContext || tickCount < 2 || scale->tick_length == 0) {
        return;
    }

    lv_area_t content{};
    lv_obj_get_content_coords(object, &content);
    const lv_coord_t edgeRadius = LV_MIN(lv_area_get_width(&content), lv_area_get_height(&content)) / 2;
    const lv_coord_t centerX = content.x1 + edgeRadius;
    const lv_coord_t centerY = content.y1 + edgeRadius;
    const lv_coord_t outerRadius = edgeRadius - 2;
    const lv_coord_t innerRadius = outerRadius - scale->tick_length;
    const lv_coord_t cap = scale->tick_width / 2;
    const bool pill = scale->tick_length > scale->tick_width;

    lv_draw_line_dsc_t line;
    lv_draw_line_dsc_init(&line);
    lv_obj_init_draw_line_dsc(object, LV_PART_TICKS, &line);
    line.width = scale->tick_width;
    line.round_start = 1;
    line.round_end = 1;
    line.raw_end = 0;
    line.opa = LV_OPA_COVER;

    lv_draw_rect_dsc_t dot;
    lv_draw_rect_dsc_init(&dot);
    dot.radius = LV_RADIUS_CIRCLE;
    dot.bg_opa = LV_OPA_COVER;
    const float dotRadius = scale->tick_length / 2.0f;
    const float dotBand = outerRadius - dotRadius;
    const lv_coord_t roundedDotRadius = static_cast<lv_coord_t>(lroundf(dotRadius));
    constexpr float degreesToRadians = 3.14159265358979323846f / 180.0f;

    for (uint16_t i = 0; i < tickCount; ++i) {
        const int32_t value = lv_map(i, 0, tickCount - 1, scale->min, scale->max);
        lv_color_t color = scale->tick_color;
        for (auto *indicator = static_cast<lv_meter_indicator_t *>(_lv_ll_get_tail(&meter->indicator_ll)); indicator;
             indicator = static_cast<lv_meter_indicator_t *>(_lv_ll_get_prev(&meter->indicator_ll, indicator))) {
            if (indicator->type != LV_METER_INDICATOR_TYPE_SCALE_LINES || value < indicator->start_value ||
                value > indicator->end_value) {
                continue;
            }
            if (indicator->type_data.scale_lines.color_start.full == indicator->type_data.scale_lines.color_end.full) {
                color = indicator->type_data.scale_lines.color_start;
            } else {
                const lv_opa_t ratio = indicator->type_data.scale_lines.local_grad
                                           ? lv_map(value, indicator->start_value, indicator->end_value, LV_OPA_TRANSP,
                                                    LV_OPA_COVER)
                                           : lv_map(value, scale->min, scale->max, LV_OPA_TRANSP, LV_OPA_COVER);
                color = lv_color_mix(indicator->type_data.scale_lines.color_end,
                                     indicator->type_data.scale_lines.color_start, ratio);
            }
        }

        const float angle = (static_cast<float>(i) * scale->angle_range / (tickCount - 1) + scale->rotation) *
                            degreesToRadians;
        const float x = cosf(angle);
        const float y = sinf(angle);
        if (pill) {
            lv_point_t inner = {static_cast<lv_coord_t>(lroundf(centerX + x * (innerRadius + cap))),
                                static_cast<lv_coord_t>(lroundf(centerY + y * (innerRadius + cap)))};
            lv_point_t outer = {static_cast<lv_coord_t>(lroundf(centerX + x * (outerRadius - cap))),
                                static_cast<lv_coord_t>(lroundf(centerY + y * (outerRadius - cap)))};
            line.color = color;
            lv_draw_line(drawContext, &line, &inner, &outer);
        } else {
            const lv_coord_t dotX = static_cast<lv_coord_t>(lroundf(centerX + x * dotBand));
            const lv_coord_t dotY = static_cast<lv_coord_t>(lroundf(centerY + y * dotBand));
            dot.bg_color = color;
            lv_area_t area = {static_cast<lv_coord_t>(dotX - roundedDotRadius),
                              static_cast<lv_coord_t>(dotY - roundedDotRadius),
                              static_cast<lv_coord_t>(dotX + roundedDotRadius),
                              static_cast<lv_coord_t>(dotY + roundedDotRadius)};
            lv_draw_rect(drawContext, &dot, &area);
        }
    }
}

void action_on_steam_temp_lower(lv_event_t *) { controller.lowerTemp(); }
void action_on_steam_temp_raise(lv_event_t *) { controller.raiseTemp(); }
void action_on_grind_time_lower(lv_event_t *) { controller.lowerGrindTarget(); }
void action_on_grind_time_raise(lv_event_t *) { controller.raiseGrindTarget(); }
void action_on_timed_click(lv_event_t *) {}
void action_on_volumetric_click(lv_event_t *) {
    controller.onTargetToggle();
    controller.getUI()->markDirty();
}
void action_on_grind_toggle(lv_event_t *) {
    controller.isGrindActive() ? controller.deactivateGrind() : controller.activateGrind();
}
void action_on_simple_process_toggle(lv_event_t *) {
    // Steam has no play/pause toggle on this path (exception preserved from legacy).
    if (controller.getMode() == MODE_STEAM) {
        return;
    }
    // Brew pause must match WideLayout: deactivate then clear (not toggle-only).
    if (controller.getMode() == MODE_BREW && controller.isActive()) {
        ProcessControl::deactivateAndClear(controller);
        return;
    }
    ProcessControl::toggleActive(controller);
}
void action_on_profile_load(lv_event_t *) { controller.getUI()->onProfileSelect(); }
void action_on_previous_profile(lv_event_t *) { controller.getUI()->onPreviousProfile(); }
void action_on_next_profile(lv_event_t *) { controller.getUI()->onNextProfile(); }
void action_on_brew_cancel(lv_event_t *) { ProcessControl::deactivateAndClear(controller); }

void action_on_standby(lv_event_t *) { controller.activateStandby(); }

void action_on_screen_load(lv_event_t *event) {
    suppressMeterTicks(lv_event_get_target(event));
    // Match legacy eez/actions.cpp rich click-area list; applyClickArea no-ops null objects.
    applyClickArea(objects.select_profile, 30);
    applyClickArea(objects.previous_profile, 30);
    applyClickArea(objects.next_profile, 30);
    applyClickArea(objects.btn_brew_1, 15);
    applyClickArea(objects.btn_steam_1, 15);
    applyClickArea(objects.btn_water_1, 15);
    applyClickArea(objects.btn_grind_1, 15);
    applyClickArea(objects.btn_settings_1, 15);
    applyClickArea(objects.info_btn, 15);
    applyClickArea(objects.menu_dials__standby_icon, 20);
    applyClickArea(objects.standby_btn, 20);
    applyClickArea(objects.brew_dials__menu_icon, 20);
    applyClickArea(objects.status_dials__menu_icon, 20);
    applyClickArea(objects.steam_dials__menu_icon, 20);
    applyClickArea(objects.water_dials__menu_icon, 20);
    applyClickArea(objects.grind_dials__menu_icon, 20);
    applyClickArea(objects.profile_dials__menu_icon, 20);
    applyClickArea(objects.info_menu_icon, 20);
    applyClickArea(objects.start_button, 25);
    applyClickArea(objects.water_start_button, 25);
    applyClickArea(objects.grind_start_button, 25);
    applyClickArea(objects.profile_select_button, 25);
    applyClickArea(objects.settings_button, 25);
    applyClickArea(objects.up_duration_button, 15);
    applyClickArea(objects.down_duration_button, 15);
    applyClickArea(objects.up_weight_button, 15);
    applyClickArea(objects.down_weight_button, 15);
    applyClickArea(objects.up_temp_button, 15);
    applyClickArea(objects.down_temp_button, 15);
    applyClickArea(objects.water_up_temp_button, 15);
    applyClickArea(objects.water_down_temp_button, 15);
    applyClickArea(objects.steam_up_temp_button, 15);
    applyClickArea(objects.steam_down_temp_button, 15);
    applyClickArea(objects.grind_up_duration_button, 15);
    applyClickArea(objects.grind_down_duration_button, 15);
    applyClickArea(objects.grind_up_weight_button, 15);
    applyClickArea(objects.grind_down_weight_button, 15);
    applyClickArea(objects.pause_button, 25);
    applyClickArea(objects.check_button, 25);
    applyClickArea(objects.accept_button, 20);
    applyClickArea(objects.save_as_new_button, 20);
    applyClickArea(objects.save_button, 20);
}

void action_on_screen_swipe(lv_event_t *event) {
    if (lv_event_get_code(event) != LV_EVENT_GESTURE) {
        return;
    }
    const lv_dir_t direction = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (direction == LV_DIR_TOP) {
        lv_indev_wait_release(lv_indev_get_act());
        action_on_menu_click(event);
    } else if (direction == LV_DIR_RIGHT && eez_flow_get_current_screen() == SCREEN_ID_PROFILE_SCREEN) {
        lv_indev_wait_release(lv_indev_get_act());
        action_on_previous_profile(event);
    } else if (direction == LV_DIR_LEFT && eez_flow_get_current_screen() == SCREEN_ID_PROFILE_SCREEN) {
        lv_indev_wait_release(lv_indev_get_act());
        action_on_next_profile(event);
    }
}

void action_on_info_screen(lv_event_t *) { controller.getUI()->changeScreen(SCREEN_ID_INFO_SCREEN); }

void action_on_manual_brew_load(lv_event_t *) { NativeManualBrew::onLoad(); }
void action_on_manual_brew_toggle(lv_event_t *) { NativeManualBrew::toggle(); }
void action_on_manual_temp_press(lv_event_t *event) { NativeManualBrew::setTemperatureFromPointer(event); }
void action_on_manual_temp_drag(lv_event_t *event) { NativeManualBrew::setTemperatureFromPointer(event); }
void action_on_manual_temp_release(lv_event_t *event) { NativeManualBrew::setTemperatureFromPointer(event); }
void action_on_manual_pressure_press(lv_event_t *event) { NativeManualBrew::setPressureFromPointer(event); }
void action_on_manual_pressure_drag(lv_event_t *event) { NativeManualBrew::setPressureFromPointer(event); }
void action_on_manual_pressure_release(lv_event_t *event) { NativeManualBrew::setPressureFromPointer(event); }
void action_on_manual_pressure_snap_endpoint(lv_event_t *event) { NativeManualBrew::setPressureFromPointer(event); }
void action_on_manual_flow_press(lv_event_t *event) { NativeManualBrew::setFlowFromPointer(event); }
void action_on_manual_flow_drag(lv_event_t *event) { NativeManualBrew::setFlowFromPointer(event); }
void action_on_manual_flow_release(lv_event_t *event) { NativeManualBrew::setFlowFromPointer(event); }
void action_on_manual_brew_save(lv_event_t *) { NativeManualBrew::save(); }
void action_on_manual_brew_discard(lv_event_t *) { NativeManualBrew::discard(); }

} // extern "C"
