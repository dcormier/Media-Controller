/*
             LUFA Library
     Copyright (C) Dean Camera, 2013.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the MediaControl project. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "MediaController.h"

/** Buffer to hold the previously generated Media Control HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevMediaControlHIDReportBuffer[sizeof(USB_MediaReport_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t MediaControl_HID_Interface =
 	{
		.Config =
			{
				.InterfaceNumber              = 0,
				.ReportINEndpoint             =
					{
						.Address              = MEDIACONTROL_HID_EPADDR,
						.Size                 = MEDIACONTROL_HID_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevMediaControlHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevMediaControlHIDReportBuffer),
			},
    };


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();

	for (;;)
	{
		HID_Device_USBTask(&MediaControl_HID_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware()
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	//Joystick_Init();
	LEDs_Init();
	Buttons_Init();
	USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&MediaControl_HID_Interface);

	USB_Device_EnableSOFEvents();

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&MediaControl_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&MediaControl_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, uint8_t* const ReportID,
                                         const uint8_t ReportType, void* ReportData, uint16_t* const ReportSize)
{
	USB_MediaReport_Data_t* MediaReport = (USB_MediaReport_Data_t*)ReportData;

	//uint8_t JoyStatus_LCL    = Joystick_GetStatus();
	uint8_t ButtonStatus_LCL = Buttons_GetStatus();

	/* Update the Media Control report with the user button presses */

	//MediaReport->Play = false;
	//MediaReport->Pause = false;
	//MediaReport->FForward = false;
	//MediaReport->Rewind = false;
	MediaReport->NextTrack     = DebouncedButtonStatusNextTrack(ButtonStatus_LCL);
	MediaReport->PreviousTrack = DebouncedButtonStatusPreviousTrack(ButtonStatus_LCL);
	MediaReport->Stop          = DebouncedButtonStatusStop(ButtonStatus_LCL);
	MediaReport->PlayPause     = DebouncedButtonStatusPlayPause(ButtonStatus_LCL);
	MediaReport->Mute          = DebouncedButtonStatusMuteUnmute(ButtonStatus_LCL);

	static enum RE_CHANGE direction = RE_CHANGE_NONE;
	static uint8_t pulses = 0;

	enum RE_CHANGE newDirection = readEncoder(ButtonStatus_LCL);

	if (direction == newDirection) {
		pulses++;

		if (pulses > 3) {
			pulses = 0;

			switch (newDirection) {
				case RE_CHANGE_DIR_CW:
				MediaReport->VolumeUp = true;
				//MediaReport->VolumeDown = false;
				break;

				case RE_CHANGE_DIR_CCW:
				//MediaReport->VolumeUp = false;
				MediaReport->VolumeDown = true;
				break;

				default:
				//MediaReport->VolumeUp = false;
				//MediaReport->VolumeDown = false;
				break;
			}
		}
	}
	else if (newDirection == RE_CHANGE_DIR_CW || newDirection == RE_CHANGE_DIR_CCW) {
		// The turn direction actually changed (rather than being NONE or UNKNOWN).
		// Change the direction we're moving and reset the counter.
		direction = newDirection;
		pulses = 1; // 1, not 0, because this is a pulse in a new direction.

		// Ignore UNKNOWN and NONE
	}

	*ReportSize = sizeof(USB_MediaReport_Data_t);
	return false;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
	// Unused (but mandatory for the HID class driver) in this demo, since there are no Host->Device reports
}
