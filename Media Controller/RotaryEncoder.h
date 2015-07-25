/*
 * RotaryEncoder.h
 *
 * Created: 2013-08-18 1:45:43 PM
 *  Author: Daniel
 */ 


#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <LUFA/Drivers/Board/Buttons.h>

enum RE_CHANGE {
	RE_CHANGE_NONE,
	RE_CHANGE_DIR_UNKNOWN,
	RE_CHANGE_DIR_CW,
	RE_CHANGE_DIR_CCW
};

uint8_t readChannel(uint8_t pin, uint8_t ButtonStatus_LCL) {
	return (ButtonStatus_LCL & pin) ? 1 : 0;
}

uint8_t lastRead(uint8_t history) {
	return history & 0b11;
}

enum RE_CHANGE readEncoder(uint8_t buttonStatus) {
	static uint8_t history = 0;
	uint8_t current = (readChannel(RE_CHANNEL_A, buttonStatus) << 1) | readChannel(RE_CHANNEL_B, buttonStatus);
	
	enum RE_CHANGE change = RE_CHANGE_DIR_UNKNOWN;

	if (current == lastRead(history)) {
		change = RE_CHANGE_NONE;
	}
	else {
		// It's not the same. Update the history.
		history = (history << 2) | current;

		/*
		This is the lookup table for the possible values.
		This the two most significant bits of the nibble
		are the previous values. The 2 least significant
		bytes are the current values. This table compares
		the previous value to the current value and gives
		the direction for that change.

		This assumes we're getting 2-channel greycode.
		Bit 0 is channel B and bit 1 is for channel A.
		This assumes that, when turned clockwise, the
		rotary encode will change channel A first.

		Because we're storing 2 values for each channel
		(the current value and the previous value), we
		get a total of 4 bits. This means there are 16
		possible values of those 4 bits. We can look at
		all 16 permutations and determine what direction
		the encoder moved to create it. This array is
		the outcome of that.
		*/

		static enum RE_CHANGE lookup[] = {
			RE_CHANGE_NONE,			// 00 00 NONE
			RE_CHANGE_DIR_CCW,		// 00 01 CCW     (channel B changed first)
			RE_CHANGE_DIR_CW,		// 00 10 CW      (channel A changed first)
			RE_CHANGE_DIR_UNKNOWN,	// 00 11 UNKNOWN (both channels changed)
			RE_CHANGE_DIR_CW,		// 01 00 CW      (channel A changed first)
			RE_CHANGE_NONE,			// 01 01 NONE
			RE_CHANGE_DIR_UNKNOWN,	// 01 10 UNKNOWN (both channels changed)
			RE_CHANGE_DIR_CCW,		// 01 11 CCW     (channel B changed first)
									// Interesting note: inverse symmetry starts here
			RE_CHANGE_DIR_CCW,		// 10 00 CCW     (channel B changed first)
			RE_CHANGE_DIR_UNKNOWN,	// 10 01 UNKNOWN (both channels changed)
			RE_CHANGE_NONE,			// 10 10 NONE
			RE_CHANGE_DIR_CW,		// 10 11 CW      (channel A changed first)
			RE_CHANGE_DIR_UNKNOWN,	// 11 00 UNKNOWN (both channels changed)
			RE_CHANGE_DIR_CW,		// 11 01 CW      (channel A changed first)
			RE_CHANGE_DIR_CCW,		// 11 10 CCW     (channel B changed first)
			RE_CHANGE_NONE			// 11 11 NONE
		};

		change = lookup[history & 0b1111];
	}
	
	return change;
}

#endif /* ROTARYENCODER_H_ */