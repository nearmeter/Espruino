#ifndef NRFCLAW_ESPR_LORA_PROFILE_H
#define NRFCLAW_ESPR_LORA_PROFILE_H
#include <stdbool.h>
#include <stdint.h>

#define NRFCLAW_LORA_DEFAULT_FREQUENCY_HZ 915000000UL
#define NRFCLAW_LORA_DEFAULT_POWER_DBM    14
#define NRFCLAW_LORA_DEFAULT_SF           7
#define NRFCLAW_LORA_DEFAULT_BW_KHZ       125
#define NRFCLAW_LORA_DEFAULT_CR           1
#define NRFCLAW_LORA_DEFAULT_SYNC_WORD    0x12U
#define NRFCLAW_LORA_DEFAULT_PREAMBLE     8U

typedef struct {
  uint32_t frequency_hz;
  int8_t power_dbm;
  uint8_t sf;
  uint16_t bw_khz;
  uint8_t cr;
  uint8_t sync_word;
  uint16_t preamble_symbols;
} nrfclaw_lora_profile_t;

void nrfclaw_lora_profile_default(nrfclaw_lora_profile_t *p);
bool nrfclaw_lora_profile_validate(nrfclaw_lora_profile_t const *p);
#endif
