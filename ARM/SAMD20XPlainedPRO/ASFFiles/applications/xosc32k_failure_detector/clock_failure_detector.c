/**
 * \file
 *
 * \brief SAM XOSC32K Runtime Failure Detector Application
 *
 * Copyright (C) 2015 Atmel Corporation. All rights reserved.
 *
 * \license
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage SAM XOSC32K Runtime Failure Detector Application
 * See \ref appdoc_main "here" for project documentation.
 * \copydetails appdoc_preface
 *
 *
 * \page appdoc_preface Overview
 * This application demonstrates a hardware XOSC32K external 32KHz crystal
 * oscillator clock source failure detector, using general purpose hardware
 * peripherals.
 */

/**
 * \page appdoc_main SAM XOSC32K Runtime Failure Detector
 *
 * Overview:
 * - \ref appdoc_sam0_xosc32k_fail_detect_intro
 * - \ref appdoc_sam0_xosc32k_fail_detect_compinfo
 * - \ref appdoc_sam0_xosc32k_fail_detect_usageinfo
 * - \ref appdoc_sam0_xosc32k_fail_detect_contactinfo
 *
 * \section appdoc_sam0_xosc32k_fail_detect_intro Introduction
 * This application demonstrates a runtime XOSC32K (external 32KHz crystal
 * oscillator) failure detector, using hardware Clock failure detection with
 * safe clock switch.
 *
 * Use XOSC32K as the clock source of GCLK2, OSCULP32K is the safe clock.
 * The GCLK2 output PIN will output 32K frequency signal, even though the
 * XOSC32K is failed.
 *
 * When a clock failure is detected, the XOSC32K clock is replaced by the safe 
 * clock in order to maintain an active clock during the XOSC32K clock failure.
 * The safe clock source is the OSCULP32K oscillator clock.
 *
 * In the example application, the GCLK2 reference clock is automatically
 * switched between the internal and external 32KHz clock sources depending on
 * the external reference availability.
 *
 * This application has been tested on following boards:
 * - SAM L22 Xplained Pro
 * - SAM C21 Xplained Pro
 *
 * \section appdoc_sam0_xosc32k_fail_detect_usageinfo Usage
 * Connect an oscilloscope to the pin:
 * - SAM L22 Xplained Pro: PA15
 * - SAM C21 Xplained Pro: PA16
 * 
 * Run the example application, then press and hold the board button to turn 
 * off the external XOSC32K crystal clock source to observe the fail-over to 
 * the internal clock source. Releasing the button will re-enable the external 
 * XOSC32K crystal.
 *
 * The board LED will be turned on when the external crystal is used, and
 *  will be turned off when the internal OSCULP32K oscillator is used due to a
 * XOSC32K failure detection.
 *
 * \section appdoc_sam0_xosc32k_fail_detect_compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for ARM.
 * Other compilers may or may not work.
 *
 * \section appdoc_sam0_xosc32k_fail_detect_contactinfo Contact Information
 * For further information, visit
 * <a href="http://www.atmel.com">http://www.atmel.com</a>.
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "conf_example.h"

/** If \c true, the XOSC32K clock will be routed out to GPIO pin */
#define ENABLE_XOSC32K_CLOCK_OUT   true

/** Configures and starts the OSCULP32K internal oscillator. */
static void init_osculp32k(void)
{
	/* Configure and enable the OSCULP32K clock source */
	struct system_clock_source_osculp32k_config osculp32k_conf;
	system_clock_source_osculp32k_get_config_defaults(&osculp32k_conf);

	system_clock_source_osculp32k_set_config(&osculp32k_conf);
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_ULP32K);

	return;
}

/** Configures and starts the XOSC32K external crystal. */
static void init_xosc32k(void)
{
	/* Configure and enable the XOSC32K clock source */
	struct system_clock_source_xosc32k_config xosc32k_conf;
	system_clock_source_xosc32k_get_config_defaults(&xosc32k_conf);

	xosc32k_conf.on_demand = false;
#if (SAML22)
	xosc32k_conf.clock_failure_detect.cfd_enable = true;
#else
	xosc32k_conf.enable_clock_failure_detector = true;
#endif
	system_clock_source_xosc32k_set_config(&xosc32k_conf);
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K);
	while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_XOSC32K));

#if (ENABLE_XOSC32K_CLOCK_OUT == true)
	/* Configure a GPIO pin as the GCLK output */
	struct system_pinmux_config clk_out_pin;
	system_pinmux_get_config_defaults(&clk_out_pin);
	clk_out_pin.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	clk_out_pin.mux_position = CONF_CLOCK_PIN_MUX;
	system_pinmux_pin_set_config(CONF_CLOCK_PIN_OUT, &clk_out_pin);
#endif

	return;
}

/**
 * \brief Simulation xosc32k connect or disconnect
 *
 * Simulation xosc32k connect or disconnect
 *
 * \param[in] connection  true represent xosc32k is connected
 */
static void sim_xosc32k_failure(bool connection)
{
	if (connection) {
		if (OSC32KCTRL->CFDCTRL.bit.CFDEN) {
			OSC32KCTRL->XOSC32K.bit.XTALEN = 1;
		}
		OSC32KCTRL->XOSC32K.bit.EN32K = 1;
		while(!system_clock_source_is_ready(
		SYSTEM_CLOCK_SOURCE_XOSC32K));
	}
	else {
		if (OSC32KCTRL->CFDCTRL.bit.CFDEN) {
			OSC32KCTRL->XOSC32K.bit.XTALEN = 0;
		}
		OSC32KCTRL->XOSC32K.bit.EN32K = 0;
#if (SAML22)
		system_clock_source_xosc32k_set_switch_back();
#endif
	}
	return;
}
/** Main application entry point. */
int main(void)
{
	system_init();
	system_flash_set_waitstates(2);

	init_osculp32k();
	init_xosc32k();

	for (;;) {
		static bool old_run_osc = true;
		bool new_run_osc =
				(port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_INACTIVE);

		/* Use LED0 to represent XOSC32K work normal or fail */
		if ((OSC32KCTRL->STATUS.bit.CLKFAIL)) {
				port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
		}
		else {
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		}
		/* Check if the XOSC32K needs to be disconnect or reconnect when the board
		 * button is pressed or released */
		if (new_run_osc != old_run_osc) {
			if (new_run_osc) {
				sim_xosc32k_failure(true);
			}
			else {
				sim_xosc32k_failure(false);
			}

			old_run_osc = new_run_osc;
		}
	}
}

