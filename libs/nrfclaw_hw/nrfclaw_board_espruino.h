#ifndef NRFCLAW_BOARD_ESPRUINO_H
#define NRFCLAW_BOARD_ESPRUINO_H

/* Board pin maps taken from github.com/nearmeter/nrfclaw. */
#if defined(NINASENSE)
  #define NRFCLAW_ESPR_BOARD_NAME "NINASENSE"
  #define P_LORA_SCK       2
  #define P_LORA_NSS       3
  #define P_LORA_MOSI      4
  #define P_LORA_MISO      5
  #define P_LORA_BUSY      6
  #define P_LORA_RXEN      7
  #define P_LORA_TXEN      8
  #define P_LORA_DIO1      9
  #define P_LORA_DIO2      10
  #define P_HALL1          11
  #define P_LIS_SCL        13
  #define P_HALL2          14
  #define P_SERIAL_TX      15
  #define P_SERIAL_RX      16
  #define P_LIS_CS         17
  #define P_LIS_SDA        18
  #define P_LORA_NRST      19
  #define P_BUTTON         21
  #define P_LIS_INT1       22
  #define P_LIS_INT2       23
  #define P_BAT            28
#elif defined(HALFMOON)
  #define NRFCLAW_ESPR_BOARD_NAME "HALFMOON"
  #define P_LORA_SCK       4
  #define P_LORA_NSS       5
  #define P_LORA_MOSI      6
  #define P_LORA_MISO      7
  #define P_LORA_BUSY      8
  #define P_LORA_RXEN      9
  #define P_LORA_TXEN      10
  #define P_LORA_DIO1      13
  #define P_LORA_DIO2      20
  #define P_HALL1          12
  #define P_LIS_SCL        28
  #define P_HALL2          31
  #define P_SERIAL_TX      15
  #define P_SERIAL_RX      16
  #define P_LIS_CS         25
  #define P_LIS_SDA        23
  #define P_LORA_NRST      21
  #define P_BUTTON         22
  #define P_LIS_INT1       29
  #define P_LIS_INT2       30
  #define P_BAT            2
#else
  #error "nrfclaw_hw requires NINASENSE or HALFMOON"
#endif

#endif
