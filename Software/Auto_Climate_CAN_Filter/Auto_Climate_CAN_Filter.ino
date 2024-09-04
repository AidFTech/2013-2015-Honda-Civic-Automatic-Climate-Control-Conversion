#include <stdint.h>
#include <mcp2515.h>
#include <SPI.h>

#include "Auto_Climate_Translator.h"

#define AC_PIN 5
#define ECON_PIN 6
#define LDAC 7
#define DAC_CS 8

MCP2515 can_car(10);
MCP2515 can_unit(9);

AutoClimateTranslator auto_translator(&can_car, &can_unit, AC_PIN, ECON_PIN, DAC_CS, LDAC); 

void setup() {
	can_car.reset();
	can_car.setBitrate(CAN_125KBPS);
	can_car.setNormalMode();
	
	can_unit.reset();
	can_unit.setBitrate(CAN_125KBPS);
	can_unit.setNormalMode();
	
	pinMode(LDAC, OUTPUT);
	pinMode(DAC_CS, OUTPUT);
	auto_translator.sendStartDAC();
}

void loop() {
	auto_translator.getCANMessages();
}
