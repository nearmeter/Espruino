#include "nrfclaw_espruino.h"
#include "nrfclaw_llcc68.h"
#include "nrfclaw_lora_profile.h"
#include "nrfclaw_battery.h"
#include "jsinteractive.h"
#include "jswrap_arraybuffer.h"
#include "nrf_delay.h"

static bool radio_initialized;
static nrfclaw_lora_profile_t current_profile;

bool nrfclaw_espr_lora_init_default(void) {
  nrfclaw_lora_profile_default(&current_profile);
  radio_initialized=nrfclaw_llcc68_init(&current_profile);
  return radio_initialized;
}

bool nrfclaw_espr_lora_config(JsVar *options) {
  if (!options || !jsvIsObject(options)) {
    jsExceptionHere(JSET_TYPEERROR,"LoRa config must be an object");
    return false;
  }

  nrfclaw_lora_profile_t p;
  if (radio_initialized) p=current_profile;
  else nrfclaw_lora_profile_default(&p);

  p.frequency_hz=(uint32_t)jsvObjectGetIntegerChildOr(options,"frequency",(JsVarInt)p.frequency_hz);
  p.power_dbm=(int8_t)jsvObjectGetIntegerChildOr(options,"power",(JsVarInt)p.power_dbm);
  p.sf=(uint8_t)jsvObjectGetIntegerChildOr(options,"sf",(JsVarInt)p.sf);
  p.bw_khz=(uint16_t)jsvObjectGetIntegerChildOr(options,"bandwidth",(JsVarInt)p.bw_khz);
  p.cr=(uint8_t)jsvObjectGetIntegerChildOr(options,"codingRate",(JsVarInt)p.cr);
  p.sync_word=(uint8_t)jsvObjectGetIntegerChildOr(options,"syncWord",(JsVarInt)p.sync_word);
  p.preamble_symbols=(uint16_t)jsvObjectGetIntegerChildOr(options,"preamble",(JsVarInt)p.preamble_symbols);

  if (!nrfclaw_lora_profile_validate(&p)) {
    jsExceptionHere(JSET_ERROR,"Invalid LoRa configuration");
    return false;
  }
  if (!radio_initialized) {
    radio_initialized=nrfclaw_llcc68_init(&p);
    if (!radio_initialized) return false;
  } else if (!nrfclaw_llcc68_apply_profile(&p)) {
    nrfclaw_llcc68_sleep();
    return false;
  }
  current_profile=p;
  return nrfclaw_llcc68_sleep();
}

bool nrfclaw_espr_lora_send(JsVar *data) {
  if (!data || !jsvIsString(data)) {
    jsExceptionHere(JSET_TYPEERROR,"LoRa payload must be a string");
    return false;
  }
  size_t len=jsvGetStringLength(data);
  if (!len || len>64U) {
    jsExceptionHere(JSET_ERROR,"LoRa payload must be 1..64 bytes");
    return false;
  }
  uint8_t buf[65];
  jsvGetString(data,(char*)buf,len+1U);
  if (!radio_initialized && !nrfclaw_espr_lora_init_default()) return false;
  if (!nrfclaw_llcc68_start_tx(buf,(uint8_t)len)) {
    nrfclaw_llcc68_sleep();
    return false;
  }
  bool ok=false;
  for (uint32_t ms=0; ms<4500U; ++ms) {
    uint16_t irq=0;
    if (!nrfclaw_llcc68_get_irq(&irq)) break;
    if (irq & NRFCLAW_LLCC68_IRQ_TX_DONE) { ok=true; break; }
    if (irq & NRFCLAW_LLCC68_IRQ_TIMEOUT) break;
    nrf_delay_ms(1);
  }
  bool finish=nrfclaw_llcc68_finish_tx();
  return ok && finish;
}

JsVar *nrfclaw_espr_lora_receive(int timeout_ms) {
  if (timeout_ms<=0) timeout_ms=1000;
  if (timeout_ms>30000) timeout_ms=30000;
  if (!radio_initialized && !nrfclaw_espr_lora_init_default()) return 0;
  if (!nrfclaw_llcc68_start_rx(true)) {
    nrfclaw_llcc68_sleep();
    return 0;
  }
  JsVar *result=0;
  for (int ms=0; ms<timeout_ms; ++ms) {
    uint16_t irq=0;
    if (!nrfclaw_llcc68_get_irq(&irq)) break;
    if (irq & NRFCLAW_LLCC68_IRQ_RX_DONE) {
      uint8_t data[64], len=0;
      int16_t rssi2=0, snr4=0;
      if (nrfclaw_llcc68_read_packet(data,&len,sizeof(data),&rssi2,&snr4)) {
        result=jsvNewObject();
        if (result) {
          JsVar *ab=jsvNewArrayBufferWithData(len,(const char*)data);
          if (ab) jsvObjectSetChildAndUnLock(result,"data",ab);
          jsvObjectSetFloatChild(result,"rssi",((JsVarFloat)rssi2)/2.0);
          jsvObjectSetFloatChild(result,"snr",((JsVarFloat)snr4)/4.0);
        }
      }
      break;
    }
    if (irq & (NRFCLAW_LLCC68_IRQ_TIMEOUT | NRFCLAW_LLCC68_IRQ_CRC_ERR |
               NRFCLAW_LLCC68_IRQ_HEADER_ERR)) break;
    nrf_delay_ms(1);
  }
  nrfclaw_llcc68_finish_rx();
  return result;
}

bool nrfclaw_espr_lora_sleep(void) {
  return !radio_initialized || nrfclaw_llcc68_sleep();
}

JsVarFloat nrfclaw_espr_battery(void) {
  uint16_t cv=0;
  if (!nrfclaw_battery_read(&cv,0)) return 0;
  return ((JsVarFloat)cv)/100.0;
}

JsVarInt nrfclaw_espr_battery_raw(void) {
  uint16_t cv=0,raw=0;
  if (!nrfclaw_battery_read(&cv,&raw)) return -1;
  return (JsVarInt)raw;
}
