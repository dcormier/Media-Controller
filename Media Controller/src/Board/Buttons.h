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
 *  \brief Board specific Buttons driver header for the Atmel USBKEY.
 *  \copydetails Group_Buttons_USBKEY
 *
 *  \note This file should not be included directly. It is automatically included as needed by the Buttons driver
 *        dispatch header located in LUFA/Drivers/Board/Buttons.h.
 */

/** \ingroup Group_Buttons
 *  \defgroup Group_Buttons_USBKEY USBKEY
 *  \brief Board specific Buttons driver header for the Atmel USBKEY.
 *
 *  Board specific Buttons driver header for the Atmel USBKEY.
 *
 *  <table>
 *    <tr><th>Name</th><th>Info</th><th>Active Level</th><th>Port Pin</th></tr>
 *    <tr><td>BUTTONS_BUTTON1</td><td>HWB Button</td><td>Low</td><td>PORTE.2</td></tr>
 *  </table>
 *
 *  @{
 */

#ifndef __BUTTONS_USER_H__
#define __BUTTONS_USER_H__

	/* Includes: */
		#include <LUFA/Common/Common.h>

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_BUTTONS_H)
			#error Do not include this file directly. Include LUFA/Drivers/Board/Buttons.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Button mask for the first button on the board. */
			enum inputs {
				BUTTON_STOP           = _BV(0),
				BUTTON_TRACK_PREVIOUS = _BV(1),
				BUTTON_TRACK_NEXT     = _BV(2),
				BUTTON_PLAY_PAUSE     = _BV(3),
				BUTTON_MUTE           = _BV(4),

				BUTTON_MASK           = (BUTTON_STOP | BUTTON_TRACK_PREVIOUS | BUTTON_TRACK_NEXT | BUTTON_PLAY_PAUSE | BUTTON_MUTE),

				RE_CHANNEL_A          = _BV(6),
				RE_CHANNEL_B          = _BV(5),

				RE_MASK               = (RE_CHANNEL_A | RE_CHANNEL_B),

				INPUT_MASK            = (BUTTON_MASK | RE_MASK)
			};

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void Buttons_Init(void)
			{
				DDRB  &= ~INPUT_MASK;
				PORTB |=  INPUT_MASK;
			}

			static inline void Buttons_Disable(void)
			{
				DDRB  &= ~INPUT_MASK;
				PORTB &= ~INPUT_MASK;
			}

			static inline uint8_t Buttons_GetStatus(void) ATTR_WARN_UNUSED_RESULT;
			static inline uint8_t Buttons_GetStatus(void)
			{
				return (PINB & INPUT_MASK) ^ INPUT_MASK;
			}

		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

