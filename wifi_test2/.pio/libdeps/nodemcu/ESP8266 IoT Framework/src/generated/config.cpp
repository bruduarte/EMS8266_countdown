#include <Arduino.h>
#include "config.h"

uint32_t configVersion = 2194856746; //generated identifier to compare config with EEPROM

const configData defaults PROGMEM =
{
	"Europe",
	"Budapest",
	"en",
	"ESP8266 Countdown"
};