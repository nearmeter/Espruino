#include "nrfclaw_battery.h"
#include "nrfclaw_board_espruino.h"

#include "jshardware.h"

bool nrfclaw_battery_read(uint16_t *centivolts, uint16_t *raw_average) {
  if (!centivolts) return false;

  JsVarFloat v = 0;

  /*
   * jshPinAnalog() returns a normalized value from 0.0 to 1.0.
   * Average multiple samples for stability.
   */
  const unsigned samples = 8;
  for (unsigned i = 0; i < samples; i++) {
    v += jshPinAnalog(P_BAT);
  }

  v /= samples;

  if (v < 0) v = 0;
  if (v > 1) v = 1;

  /*
   * Espruino nRF52 normalized ADC:
   * jshPinAnalog() internally converts SAADC reading using /16384.0.
   *
   * Preserve an equivalent pseudo-raw value for batteryRaw().
   */
  uint16_t raw = (uint16_t)(v * 16384.0 + 0.5);

  /*
   * NINASENSE/HALFMOON battery divider calibration:
   *   VADC full scale = 3.0 V on the board rail
   *   VBAT = VADC * 1.402
   */
  uint32_t vadc_mv = (uint32_t)(v * 3000.0 + 0.5);
  uint32_t vbat_mv = (vadc_mv * 14020UL + 5000UL) / 10000UL;

  *centivolts = (uint16_t)((vbat_mv + 5UL) / 10UL);

  if (raw_average)
    *raw_average = raw;

  return true;
}
