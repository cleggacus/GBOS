#ifndef SETTINGS_H
#define SETTINGS_H

enum SettingOptions {
    SETTINGS_WIFI,
    SETTINGS_TIME,
    SETTINGS_BACK
};

void init_settings(void);
void update_settings(void);

#endif
