#ifndef NRFCLAW_ESPR_LLCC68_H
#define NRFCLAW_ESPR_LLCC68_H
#include <stdbool.h>
#include <stdint.h>
#include "nrfclaw_lora_profile.h"

typedef enum {
  NRFCLAW_LLCC68_IRQ_NONE       = 0x0000,
  NRFCLAW_LLCC68_IRQ_TX_DONE    = 0x0001,
  NRFCLAW_LLCC68_IRQ_RX_DONE    = 0x0002,
  NRFCLAW_LLCC68_IRQ_PREAMBLE   = 0x0004,
  NRFCLAW_LLCC68_IRQ_HEADER_OK  = 0x0010,
  NRFCLAW_LLCC68_IRQ_HEADER_ERR = 0x0020,
  NRFCLAW_LLCC68_IRQ_CRC_ERR    = 0x0040,
  NRFCLAW_LLCC68_IRQ_TIMEOUT    = 0x0200
} nrfclaw_llcc68_irq_t;

bool nrfclaw_llcc68_init(nrfclaw_lora_profile_t const *profile);
bool nrfclaw_llcc68_apply_profile(nrfclaw_lora_profile_t const *profile);
bool nrfclaw_llcc68_wakeup(void);
bool nrfclaw_llcc68_standby(void);
bool nrfclaw_llcc68_sleep(void);
bool nrfclaw_llcc68_start_tx(uint8_t const *data, uint8_t len);
bool nrfclaw_llcc68_finish_tx(void);
bool nrfclaw_llcc68_start_rx(bool infinite);
bool nrfclaw_llcc68_rearm_rx(void);
bool nrfclaw_llcc68_finish_rx(void);
bool nrfclaw_llcc68_get_irq(uint16_t *irq);
bool nrfclaw_llcc68_clear_irq(uint16_t mask);
bool nrfclaw_llcc68_read_packet(uint8_t *data, uint8_t *len, uint8_t max_len,
                                int16_t *rssi_dbm_x2, int16_t *snr_db_x4);
#endif
