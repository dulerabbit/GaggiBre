// Manual Brew screen (custom)

#ifndef UI_MANUALBREWSCREEN_H
#define UI_MANUALBREWSCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

extern void ui_ManualBrewScreen_screen_init(void);
extern void ui_ManualBrewScreen_screen_destroy(void);
extern void ui_event_ManualBrewScreen(lv_event_t *e);
extern lv_obj_t *ui_ManualBrewScreen;
extern lv_obj_t *ui_ManualBrewScreen_dials;
extern lv_obj_t *ui_ManualBrewScreen_tempZone;
extern lv_obj_t *ui_ManualBrewScreen_pressureZone;

extern void ui_event_ManualBrewScreen_backButton(lv_event_t *e);
extern lv_obj_t *ui_ManualBrewScreen_backButton;
extern lv_obj_t *ui_ManualBrewScreen_contentPanel;
extern lv_obj_t *ui_ManualBrewScreen_mainLabel;

extern lv_obj_t          *ui_ManualBrewScreen_chart;
extern lv_chart_series_t *ui_ManualBrewScreen_chart_pressure;
extern lv_chart_series_t *ui_ManualBrewScreen_chart_temp;
extern lv_chart_series_t *ui_ManualBrewScreen_chart_flow;

extern lv_obj_t *ui_ManualBrewScreen_pressureCircle;
extern lv_obj_t *ui_ManualBrewScreen_pressureTarget;

extern lv_obj_t *ui_ManualBrewScreen_bottomRow;
extern lv_obj_t *ui_ManualBrewScreen_targetTemp;
extern lv_obj_t *ui_ManualBrewScreen_elapsed;
extern lv_obj_t *ui_ManualBrewScreen_flow;
extern lv_obj_t *ui_ManualBrewScreen_weight;

extern void ui_event_ManualBrewScreen_startButton(lv_event_t *e);
extern lv_obj_t *ui_ManualBrewScreen_startButton;
extern void ui_event_ManualBrewScreen_tempZone(lv_event_t *e);
extern void ui_event_ManualBrewScreen_pressureZone(lv_event_t *e);

extern void ui_event_ManualBrewScreen_saveButton(lv_event_t *e);
extern void ui_event_ManualBrewScreen_discardButton(lv_event_t *e);
extern lv_obj_t *ui_ManualBrewScreen_savePanel;
extern lv_obj_t *ui_ManualBrewScreen_saveNameLabel;
extern lv_obj_t *ui_ManualBrewScreen_saveButton;
extern lv_obj_t *ui_ManualBrewScreen_discardButton;

extern lv_obj_t *uic_ManualBrewScreen_dials_tempGauge;
extern lv_obj_t *uic_ManualBrewScreen_dials_tempTarget;
extern lv_obj_t *uic_ManualBrewScreen_dials_pressureGauge;
extern lv_obj_t *uic_ManualBrewScreen_dials_pressureTarget;
extern lv_obj_t *uic_ManualBrewScreen_dials_pressureText;
extern lv_obj_t *uic_ManualBrewScreen_dials_tempText;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
