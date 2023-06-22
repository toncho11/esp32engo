/*

Implementation of ENGO's commands as C++ functions.

*/

#ifndef ENGO_COMMANDS_H
#define ENGO_COMMANDS_H

#include "inttypes.h"

//const char* TAG_BLE = "esp32engo_commands";

void Text(int16_t x, int16_t y, uint8_t r, uint8_t f, uint8_t c, const char* text, uint8_t** command, size_t* length)
{
	size_t str_length = strlen(text);
	
	*length = 12 + str_length; //total length of command
	
	//*command = (uint8_t*)malloc(*length);
	
	(*command)[1] = 0x37; //txt command
	
	(*command)[2] = 0; //TODO: calculate the text length and decide if the length of the command is 1 or 2 bytes
	
	(*command)[3] = *length;
	
	//next is the data for the command - parameters for the text 
	
	//takes 2 bytes, to big endian mode by byte swap
    (*command)[4] = (x >> 8) & 0xFF; //second byte as first one
    (*command)[5] = (x >> 0) & 0xFF; 
	
	//takes 2 bytes, to big endian mode by byte swap
	(*command)[6] = (y >> 8) & 0xFF;
    (*command)[7] = (y >> 0) & 0xFF;
	
	(*command)[8]  = r;
	(*command)[9]  = f;
	(*command)[10] = c;
	
	memcpy((*command) + 11, text, str_length); 
}

void Demo(uint8_t demo_id, uint8_t** command, size_t* length)
{
	//*command = (uint8_t*)malloc(6);
	
	(*command)[1] = 0x03; //command id - demo now
	(*command)[2] = 0; //Command Format
	//bit 5 is the size of the length, must be 0 for demo
	//bit 4 to 1 defines the size of the query id
	//query id must be set to 0 for the demo command, because there is no query id
	
	(*command)[3] = 6; //size of all is 6 now
	(*command)[4] = demo_id; //data for the command - parameter for the demo in this case
	
	*length = 6; //must also reserve 2 for header / footer		
}

void clear_screen(uint8_t** command, size_t* length)
{
	*length = 5;
	//*command = (uint8_t*)malloc(*length);
	(*command)[1] = 0x1; //clear command
	(*command)[2] = 0; 
	(*command)[3] = *length;
}
#endif
