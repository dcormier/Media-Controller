/*
 * DebounceButtons.h
 *
 * Created: 2013-09-28 9:41:29 AM
 *  Author: Daniel
 */ 


#ifndef DEBOUNCEBUTTONS_H_
#define DEBOUNCEBUTTONS_H_

#include <stdbool.h>
#include <LUFA/Drivers/Board/Buttons.h>

bool ButtonStatus(uint8_t ButtonStatus_LCL, enum inputs button);

bool DebouncedButtonStatusPlayPause(uint8_t ButtonStatus_LCL);
bool DebouncedButtonStatusNextTrack(uint8_t ButtonStatus_LCL);
bool DebouncedButtonStatusPreviousTrack(uint8_t ButtonStatus_LCL);
bool DebouncedButtonStatusStop(uint8_t ButtonStatus_LCL);
bool DebouncedButtonStatusMuteUnmute(uint8_t ButtonStatus_LCL);

#endif /* DEBOUNCEBUTTONS_H_ */