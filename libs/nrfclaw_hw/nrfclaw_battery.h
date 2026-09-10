#ifndef NRFCLAW_ESPR_BATTERY_H
#define NRFCLAW_ESPR_BATTERY_H
#include <stdbool.h>
#include <stdint.h>
bool nrfclaw_battery_read(uint16_t *centivolts, uint16_t *raw_average);
#endif
