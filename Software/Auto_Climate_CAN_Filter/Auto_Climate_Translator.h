#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <mcp2515.h>
#include <MCP_DAC.h>

#ifndef auto_climate_translator_h
#define auto_climate_translator_h

#define TEMP_DISPLAY_ID 0x92F96250
#define ECON_ID 0x92F85050
#define CLIMATE_MSG_1 0x92F85351
#define CLIMATE_MSG_2 0x92F85851
#define CLIMATE_MSG_3 0x9610FF51

#define ECONOMY_AVAILABLE //Comment out for Si.

class AutoClimateTranslator {
public:
	AutoClimateTranslator(MCP2515* car_can, MCP2515* unit_can, uint8_t ac_pin, uint8_t econ_pin, uint8_t dac_cs, uint8_t dac_latch);
	
	void getCANMessages();
	void sendStartDAC();
	
private:
	MCP2515* car_can;
	MCP2515* unit_can;
	
	MCP4921 dac_mcp;
	
	uint8_t ac_pin, econ_pin, dac_cs, dac_latch;
	
	void interpretCarMessage(can_frame* the_message);
	void interpretUnitMessage(can_frame* the_message);
	
	uint16_t getTempVoltage(int16_t temp, const bool fahrenheit);
};

#endif
