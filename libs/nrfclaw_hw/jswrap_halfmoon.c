#include "nrfclaw_espruino.h"
/*JSON{
  "type":"class",
  "class":"HALFMOON",
  "ifdef":"HALFMOON"
}
HALFMOON Rev 1.0 native hardware helpers.
*/
/*JSON{
  "type":"staticmethod",
  "class":"HALFMOON",
  "name":"loraInit",
  "ifdef":"HALFMOON",
  "generate":"nrfclaw_espr_lora_init_default",
  "return":["bool","True on success"]
}
Initialise LLCC68 with the default profile.
*/
/*JSON{
  "type":"staticmethod",
  "class":"HALFMOON",
  "name":"loraConfig",
  "ifdef":"HALFMOON",
  "generate":"nrfclaw_espr_lora_config",
  "params":[["options","JsVar","Configuration object: {frequency,power,sf,bandwidth,codingRate,syncWord,preamble}"]],
  "return":["bool","True on success"]
}
Configure LLCC68 from a JavaScript object. Any omitted field keeps the current/default value.
*/
/*JSON{
  "type":"staticmethod",
  "class":"HALFMOON",
  "name":"loraSend",
  "ifdef":"HALFMOON",
  "generate":"nrfclaw_espr_lora_send",
  "params":[["data","JsVar","String, 1..64 bytes"]],
  "return":["bool","True on success"]
}
Blocking LoRa transmit, then radio sleep.
*/
/*JSON{
  "type":"staticmethod",
  "class":"HALFMOON",
  "name":"loraReceive",
  "ifdef":"HALFMOON",
  "generate":"nrfclaw_espr_lora_receive",
  "params":[["timeout","int","Timeout in ms"]],
  "return":["JsVar","{data:ArrayBuffer,rssi,snr} or undefined"]
}
Blocking LoRa receive, then radio sleep.
*/
/*JSON{
  "type":"staticmethod",
  "class":"HALFMOON",
  "name":"loraSleep",
  "ifdef":"HALFMOON",
  "generate":"nrfclaw_espr_lora_sleep",
  "return":["bool","True on success"]
}
Put LLCC68 in retained warm sleep and release SPI pins.
*/
/*JSON{
  "type":"staticmethod",
  "class":"HALFMOON",
  "name":"battery",
  "ifdef":"HALFMOON",
  "generate":"nrfclaw_espr_battery",
  "return":["float","Battery voltage in volts"]
}
Read battery voltage.
*/
/*JSON{
  "type":"staticmethod",
  "class":"HALFMOON",
  "name":"batteryRaw",
  "ifdef":"HALFMOON",
  "generate":"nrfclaw_espr_battery_raw",
  "return":["int","14-bit normalized averaged ADC value"]
}
Return raw averaged battery ADC.
*/
