#!/bin/false
import pinutils

info = {
  'name' : 'HALFMOON Rev 1.0',
  'link' : [ 'https://github.com/nearmeter/nrfclaw' ],
  'default_console' : 'EV_BLUETOOTH',
  'variables' : 2400,
  'binary_name' : 'espruino_%v_halfmoon.hex',
  'build' : {
    'optimizeflags' : '-Os',
    'libraries' : [ 'BLUETOOTH' ],
    'makefile' : [
      'NRF_SDK17=1',
      'DEFINES+=-DHALFMOON',
      'DEFINES+=-DBLUETOOTH_NAME_PREFIX=\'"HALFMOON"\'',
      'DEFINES+=-DBOARD_PCA10040 -DPCA10040',
      'DEFINES+=-DCONFIG_NFCT_PINS_AS_GPIOS',
      'DEFINES+=-DESPR_DCDC_ENABLE',
      'DEFINES+=-DESPR_LSE_ENABLE',
      'DEFINES+=-DNRF_BLE_GATT_MAX_MTU_SIZE=53 -DNRF_BLE_MAX_MTU_SIZE=53',
      'DEFINES+=-DCENTRAL_LINK_COUNT=2 -DNRF_SDH_BLE_CENTRAL_LINK_COUNT=2',
      'LDFLAGS+=-Xlinker --defsym=LD_APP_RAM_BASE=0x3290',
      'LDFLAGS+=-nostartfiles',
      'ASFLAGS+=-D__STARTUP_CLEAR_BSS -D__START=main',
      'INCLUDE+=-I$(ROOT)/libs/nrfclaw_hw',
      'WRAPPERSOURCES+=libs/nrfclaw_hw/jswrap_halfmoon.c',
      'SOURCES+=libs/nrfclaw_hw/nrfclaw_espruino.c',
      'SOURCES+=libs/nrfclaw_hw/nrfclaw_llcc68.c',
      'SOURCES+=libs/nrfclaw_hw/nrfclaw_lora_profile.c',
      'SOURCES+=libs/nrfclaw_hw/nrfclaw_battery.c',
    ]
  }
}

chip = {
  'part':'NRF52832', 'family':'NRF52', 'package':'QFN48',
  'ram':64, 'flash':512, 'speed':64,
  'usart':1, 'spi':1, 'i2c':1, 'adc':1, 'dac':0,
  'saved_code': {
    'address':0x6C000, 'page_size':4096, 'pages':10,
    'flash_available':280
  }
}

devices = {
  'BTN1' : { 'pin':'D22', 'pinstate':'IN_PULLUP' },
  'RX_PIN_NUMBER' : { 'pin':'D16' },
  'TX_PIN_NUMBER' : { 'pin':'D15' },
  'LORA' : {
    'device':'LLCC68', 'pin_sck':'D4', 'pin_nss':'D5', 'pin_mosi':'D6',
    'pin_miso':'D7', 'pin_busy':'D8', 'pin_rxen':'D9', 'pin_txen':'D10',
    'pin_dio1':'D13', 'pin_dio2':'D20', 'pin_rst':'D21'
  },
  'ACCEL' : {
    'device':'LIS2DH12', 'addr':0x18, 'pin_scl':'D28', 'pin_sda':'D23',
    'pin_cs':'D25', 'pin_int1':'D29', 'pin_int2':'D30'
  },
  'BAT' : { 'pin_voltage':'D2' }
}

board = {
  'left' : ['D2','D4','D5','D6','D7','D8','D9','D10','D12','D13'],
  'right': ['D15','D16','D17','D20','D21','D22','D23','D25','D26','D27','D28','D29','D30','D31'],
  '_notes' : {
    'D2':'Battery ADC', 'D4':'LLCC68 SCK', 'D5':'LLCC68 NSS', 'D6':'LLCC68 MOSI',
    'D7':'LLCC68 MISO', 'D8':'LLCC68 BUSY', 'D9':'LLCC68 RXEN', 'D10':'LLCC68 TXEN',
    'D12':'HALL1', 'D13':'LLCC68 DIO1', 'D15':'Serial1 TX', 'D16':'Serial1 RX',
    'D17':'Analog/free', 'D20':'LLCC68 DIO2', 'D21':'LLCC68 NRST', 'D22':'Recovery/DFU button',
    'D23':'LIS2DH12 SDA', 'D25':'LIS2DH12 CS', 'D26':'Analog/free', 'D27':'Analog/free',
    'D28':'LIS2DH12 SCL', 'D29':'LIS2DH12 INT1', 'D30':'LIS2DH12 INT2', 'D31':'HALL2'
  }
}
board['_css'] = ''

def get_pins():
  pins=pinutils.generate_pins(0,31)
  pinutils.findpin(pins,'PD0',True)['functions']['XL1']=0
  pinutils.findpin(pins,'PD1',True)['functions']['XL2']=0
  pinutils.findpin(pins,'PD15',True)['functions']['TXD']=0
  pinutils.findpin(pins,'PD16',True)['functions']['RXD']=0
  for p,ch in [(2,0),(3,1),(4,2),(5,3),(28,4),(29,5),(30,6),(31,7)]:
    pinutils.findpin(pins,'PD%d'%p,True)['functions']['ADC1_IN%d'%ch]=0
  for pin in pins: pin['functions']['3.3']=0
  return pins
