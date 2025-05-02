#ifndef EDGE_H
#define EDGE_H

void setup_wifi(const char *ssid, const char *password);
void reconnect_Mqtt();
void setupTime();
uint64_t unixMilis();
uint64_t parseUint64(String s);

extern const long gmtOffset_sec;
extern const int daylightOffset_sec;
extern const char *ntpServer;
extern uint64_t T0NTP;
extern uint64_t T0Milis;

#endif // EDGE_H