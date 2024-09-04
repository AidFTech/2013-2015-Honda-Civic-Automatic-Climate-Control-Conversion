#include "Auto_Climate_Translator.h"

AutoClimateTranslator::AutoClimateTranslator(MCP2515* car_can, MCP2515* unit_can, uint8_t ac_pin, uint8_t econ_pin, uint8_t dac_cs, uint8_t dac_latch) {
	this->car_can = car_can;
	this->unit_can = unit_can;
	
	this->ac_pin = ac_pin;
	this->econ_pin = econ_pin;
	
	this->dac_cs = dac_cs;
	this->dac_latch = dac_latch;
	
	pinMode(this->ac_pin, OUTPUT);
	digitalWrite(this->ac_pin, LOW);
	
	#ifdef ECONOMY_AVAILABLE
	pinMode(this->econ_pin, INPUT);
	#endif
}

void AutoClimateTranslator::getCANMessages() {
	bool msg_received = false;
	can_frame can_msg;
	
	//Check car messages first.
	if(car_can->readMessage(&can_msg) == MCP2515::ERROR_OK)
		msg_received = true;
	
	if(msg_received) {
		interpretCarMessage(&can_msg);
		unit_can->sendMessage(&can_msg);
	}
	
	//Check unit messages.
	msg_received = false;
	
	if(unit_can->readMessage(&can_msg) == MCP2515::ERROR_OK)
		msg_received = true;
	
	if(msg_received) {
		interpretUnitMessage(&can_msg);
		if(can_msg.can_id != CLIMATE_MSG_1 && can_msg.can_id != CLIMATE_MSG_2 && can_msg.can_id != CLIMATE_MSG_3)
			car_can->sendMessage(&can_msg);
	}
}

void AutoClimateTranslator::sendStartDAC() {
	dac_mcp.begin(this->dac_cs);
	dac_mcp.fastWriteA(1652);
	
	pinMode(this->dac_latch, OUTPUT);
	digitalWrite(this->dac_latch, LOW);
}

void AutoClimateTranslator::interpretCarMessage(can_frame* the_message) {
	if(the_message->can_id == TEMP_DISPLAY_ID) {
		const int16_t temp = the_message->data[2] - 0x28;
		const bool fahrenheit = ((the_message->data[0]&(0b1<<5)) != 0);
		
		uint16_t dac_temp = getTempVoltage(temp, fahrenheit);
		if((dac_temp&0xF000) != 0)
			dac_temp &= 0xFFF;
		
		dac_mcp.fastWriteA(dac_temp);
	} 
	#ifdef ECONOMY_AVAILABLE
	else if(the_message->can_id == ECON_ID) {
		if(digitalRead(this->econ_pin) == HIGH) //Economy mode is on.
			the_message->data[3] = the_message->data[3]|0x20;
	}
	#endif
}

void AutoClimateTranslator::interpretUnitMessage(can_frame* the_message) {
	if(the_message->can_id == CLIMATE_MSG_1) {
		const bool ac_prohibit = ((the_message->data[5]&0x1) != 0);
		
		if(ac_prohibit)
			digitalWrite(ac_pin, LOW);
		else
			digitalWrite(ac_pin, HIGH);
	}
}

uint16_t AutoClimateTranslator::getTempVoltage(int16_t temp, const bool fahrenheit) {
	if(!fahrenheit)
		temp = temp*2;
	else
		temp = (temp - 32) * 10 / 9;
	
	//TODO: Adjust for more extreme temps.
	if(temp > 80) {
		const int32_t m = -21, b = 2752;
		return temp*m+b;
	} else if(temp < 20 && temp >= -20) {
		const int32_t m = -23, b = 2752;
		return temp*m+b;
	} else if(temp < -20) {
		const int32_t m = -24, b = 2752;
		return temp*m+b;
	} else {
		const int32_t m = -22, b = 2752;
		return temp*m+b;
	}
}
