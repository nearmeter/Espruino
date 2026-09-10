#ifndef NRFCLAW_ESPRUINO_API_H
#define NRFCLAW_ESPRUINO_API_H
#include "jsvar.h"
#include <stdbool.h>
#include <stdint.h>

bool nrfclaw_espr_lora_init_default(void);
bool nrfclaw_espr_lora_config(JsVar *options);
bool nrfclaw_espr_lora_send(JsVar *data);
JsVar *nrfclaw_espr_lora_receive(int timeout_ms);
bool nrfclaw_espr_lora_sleep(void);
JsVarFloat nrfclaw_espr_battery(void);
JsVarInt nrfclaw_espr_battery_raw(void);
#endif
