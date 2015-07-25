/*
 * DebouncedButtons.c
 *
 * Created: 2013-09-28 9:54:58 AM
 *  Author: Daniel
 */ 

#include "DebounceButtons.h"

#define DEBOUNCE_COUNT 5
#define DEBOUNCE_MUTE_COUNT 50

bool ButtonStatus(uint8_t ButtonStatus_LCL, enum inputs button) {
	return ((ButtonStatus_LCL & button) ? true : false);
}

bool DebouncedButtonStatusPlayPause(uint8_t ButtonStatus_LCL) {
	static bool knownStatus = false;
	static bool lastStatus = false;
	static uint8_t lastStatusCount = DEBOUNCE_COUNT;

	bool currentStatus = ButtonStatus(ButtonStatus_LCL, BUTTON_PLAY_PAUSE);

	if (currentStatus != knownStatus) {
		if (currentStatus == lastStatus) {
			if (lastStatusCount >= DEBOUNCE_COUNT) {
				knownStatus = currentStatus;
			}
			else {
				lastStatusCount++;
			}
		}
		else {
			lastStatus = currentStatus;
			lastStatusCount = 0;
		}
	}

	return knownStatus;
}

bool DebouncedButtonStatusNextTrack(uint8_t ButtonStatus_LCL) {
	static bool knownStatus = false;
	static bool lastStatus = false;
	static uint8_t lastStatusCount = DEBOUNCE_COUNT;

	bool currentStatus = ButtonStatus(ButtonStatus_LCL, BUTTON_TRACK_NEXT);

	if (currentStatus != knownStatus) {
		if (currentStatus == lastStatus) {
			if (lastStatusCount >= DEBOUNCE_COUNT) {
				knownStatus = currentStatus;
			}
			else {
				lastStatusCount++;
			}
		}
		else {
			lastStatus = currentStatus;
			lastStatusCount = 0;
		}
	}

	return knownStatus;
}

bool DebouncedButtonStatusPreviousTrack(uint8_t ButtonStatus_LCL) {
	static bool knownStatus = false;
	static bool lastStatus = false;
	static uint8_t lastStatusCount = DEBOUNCE_COUNT;

	bool currentStatus = ButtonStatus(ButtonStatus_LCL, BUTTON_TRACK_PREVIOUS);

	if (currentStatus != knownStatus) {
		if (currentStatus == lastStatus) {
			if (lastStatusCount >= DEBOUNCE_COUNT) {
				knownStatus = currentStatus;
			}
			else {
				lastStatusCount++;
			}
		}
		else {
			lastStatus = currentStatus;
			lastStatusCount = 0;
		}
	}

	return knownStatus;
}

bool DebouncedButtonStatusStop(uint8_t ButtonStatus_LCL) {
	static bool knownStatus = false;
	static bool lastStatus = false;
	static uint8_t lastStatusCount = DEBOUNCE_COUNT;

	bool currentStatus = ButtonStatus(ButtonStatus_LCL, BUTTON_STOP);

	if (currentStatus != knownStatus) {
		if (currentStatus == lastStatus) {
			if (lastStatusCount >= DEBOUNCE_COUNT) {
				knownStatus = currentStatus;
			}
			else {
				lastStatusCount++;
			}
		}
		else {
			lastStatus = currentStatus;
			lastStatusCount = 0;
		}
	}

	return knownStatus;
}

bool DebouncedButtonStatusMuteUnmute(uint8_t ButtonStatus_LCL) {
	static bool knownStatus = false;
	static bool lastStatus = false;
	static uint8_t lastStatusCount = DEBOUNCE_MUTE_COUNT;

	bool currentStatus = ButtonStatus(ButtonStatus_LCL, BUTTON_MUTE);

	if (currentStatus != knownStatus) {
		if (currentStatus == lastStatus) {
			if (lastStatusCount >= DEBOUNCE_COUNT) {
				knownStatus = currentStatus;
			}
			else {
				lastStatusCount++;
			}
		}
		else {
			lastStatus = currentStatus;
			lastStatusCount = 0;
		}
	}

	return knownStatus;
}
