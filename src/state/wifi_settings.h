#ifndef WIFI_SETTINGS_H
#define WIFI_SETTINGS_H

typedef enum WifiState {
    Scanning,
    Scanned
} WifiState;

void init_wifi_settings(void);
void update_wifi_settings(void);


#endif
