// GaggiBre — dedicated profile settings screen (round + shared symbols)
#ifndef UI_PROFILESETTINGSSCREEN_H
#define UI_PROFILESETTINGSSCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

extern void ui_ProfileSettingsScreen_screen_init(void);
extern void ui_ProfileSettingsScreen_screen_destroy(void);
extern void ui_event_ProfileSettingsScreen(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_backButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_downTempButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_upTempButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_downDurationButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_upDurationButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_saveButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_acceptButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_saveAsNewButton(lv_event_t *e);
extern void ui_event_ProfileSettingsScreen_adaptiveLabel(lv_event_t *e);

extern lv_obj_t *ui_ProfileSettingsScreen;
extern lv_obj_t *ui_ProfileSettingsScreen_dials;
extern lv_obj_t *ui_ProfileSettingsScreen_backButton;
extern lv_obj_t *ui_ProfileSettingsScreen_titleLabel;
extern lv_obj_t *ui_ProfileSettingsScreen_profileName;
extern lv_obj_t *ui_ProfileSettingsScreen_adaptiveLabel;
extern lv_obj_t *ui_ProfileSettingsScreen_adjustments;
extern lv_obj_t *ui_ProfileSettingsScreen_targetTemp;
extern lv_obj_t *ui_ProfileSettingsScreen_downTempButton;
extern lv_obj_t *ui_ProfileSettingsScreen_upTempButton;
extern lv_obj_t *ui_ProfileSettingsScreen_targetDuration;
extern lv_obj_t *ui_ProfileSettingsScreen_downDurationButton;
extern lv_obj_t *ui_ProfileSettingsScreen_upDurationButton;
extern lv_obj_t *ui_ProfileSettingsScreen_saveButton;
extern lv_obj_t *ui_ProfileSettingsScreen_acceptButton;
extern lv_obj_t *ui_ProfileSettingsScreen_saveAsNewButton;

extern lv_obj_t *uic_ProfileSettingsScreen_dials_tempGauge;
extern lv_obj_t *uic_ProfileSettingsScreen_dials_tempTarget;
extern lv_obj_t *uic_ProfileSettingsScreen_dials_pressureGauge;
extern lv_obj_t *uic_ProfileSettingsScreen_dials_pressureTarget;
extern lv_obj_t *uic_ProfileSettingsScreen_dials_pressureText;
extern lv_obj_t *uic_ProfileSettingsScreen_dials_tempText;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
