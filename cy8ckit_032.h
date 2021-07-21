/******************************************************************************
 * \file cy8ckit_032.h
 *
 * Description: This file contains the public interface for the CY8CKIT-032
 * Analog Front End shield.
 *
 *******************************************************************************
 * \copyright
 * Copyright 2018-2021 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cybsp.h"
#include "cy_result.h"
#include "mtb_ssd1306.h"

/**
 * \defgroup group_board_libs_pins Pins
 * \defgroup group_board_libs_afe Analog Front End
 * \defgroup group_board_libs_display OLED Display
 */

/**
 * \addtogroup group_board_libs_display
 * \{
 * The display is handled by the display-oled-ssd1306 library, details are available at
 * https://github.com/cypresssemiconductorco/display-oled-ssd1306.
 * \}
 */

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * \addtogroup group_board_libs_pins
 * \{
 * Pin mapping of the GPIOs used by shield peripherals.
 */

/** Analog pin to read the Ambient Light Sensor's raw value. */
#define CY8CKIT_032_PIN_LIGHT_SENSOR    (CYBSP_A0)
/** Analog pin to read the DAC's output voltage. */
#define CY8CKIT_032_PIN_DAC_VOLTAGE     (CYBSP_A1)
/** Analog pin to read the Potentiometer's output voltage. */
#define CY8CKIT_032_PIN_POTENTIOMETER   (CYBSP_A2)
/** UART RX pin (UART functionality is not implemented in the default PSoC firmware). */
#define CY8CKIT_032_PIN_UART_RX         (CYBSP_D0)
/** UART TX pin (UART functionality is not implemented in the default PSoC firmware). */
#define CY8CKIT_032_PIN_UART_TX         (CYBSP_D1)
/** GPIO pin (GPIO functionality is not implemented in the default PSoC firmware). */
#define CY8CKIT_032_PIN_GPIO            (CYBSP_D2)
/** Pin for Mechanical Button MB1 */
#define CY8CKIT_032_PIN_BUTTON1         (CYBSP_D4)
/** Pin for LED1 */
#define CY8CKIT_032_PIN_LED1            (CYBSP_D5)
/** Pin for LED1 */
#define CY8CKIT_032_PIN_LED2            (CYBSP_D11)
/** Pin for Mechanical Button MB2 */
#define CY8CKIT_032_PIN_BUTTON2         (CYBSP_D12)
/** Pin for CapSense Interrupt signal */
#define CY8CKIT_032_PIN_CAPSENSE_ISR    (CYBSP_D13)
/** Pin for I2C SDA */
#define CY8CKIT_032_PIN_I2C_SDA         (CYBSP_D14)
/** Pin for I2C SCL */
#define CY8CKIT_032_PIN_I2C_SCL         (CYBSP_D15)

/** \} group_board_libs_pins */

/**
 * \addtogroup group_board_libs_afe
 * \{
 * APIs for interacting with the sensors and user interface widgets on the board.
 */

/** Different operating mode for the CapSense LEDs (CLED0, CLED1, CLED2, CLED3). \ref
   cy8ckit_032_set_led_control() */
typedef enum
{
    CY8CKIT_032_LED_CTRL_CAPSENSE = 0x00, /**< LEDs are controlled by CapSense signal detect */
    CY8CKIT_032_LED_CTRL_SW       = 0x01  /**< LEDs are controlled by sofware */
                                          /**< \ref cy8ckit_032_set_cled_states() */
} cy8ckit_032_led_ctrl_t;

/** The CapSense LEDs that can be controlled by the CapSense detection, or software. See:
 *     - \ref cy8ckit_032_set_led_control()
 *     - \ref cy8ckit_032_get_cled_states()
 *     - \ref cy8ckit_032_get_cled_state()
 *     - \ref cy8ckit_032_set_cled_states()
 *     - \ref cy8ckit_032_set_cled_state()
 */
typedef enum
{
    CY8CKIT_032_CLED0 = 0x01, /**< CapSense LED0 */
    CY8CKIT_032_CLED1 = 0x02, /**< CapSense LED1 */
    CY8CKIT_032_CLED2 = 0x04, /**< CapSense LED2 */
    CY8CKIT_032_CLED3 = 0x08  /**< CapSense LED3 */
} cy8ckit_032_cled_t;

/** The different buttons that are read by \ref cy8ckit_032_get_button_states() or \ref
   cy8ckit_032_get_button_state() */
typedef enum
{
    CY8CKIT_032_BTN_CAPSENSE_B0   = 0x01, /**< CapSense Button 0 (B0) */
    CY8CKIT_032_BTN_CAPSENSE_B1   = 0x02, /**< CapSense Button 1 (B1) */
    CY8CKIT_032_BTN_CAPSENSE_B2   = 0x04, /**< CapSense Button 2 (B2) */
    CY8CKIT_032_BTN_CAPSENSE_B3   = 0x08, /**< CapSense Button 3 (B3) */
    CY8CKIT_032_BTN_MECHANICAL_B1 = 0x10, /**< Mechanical Button 1 (MB1) */
    CY8CKIT_032_BTN_MECHANICAL_B2 = 0x20, /**< Mechanical Button 2 (MB2) */
    CY8CKIT_032_BTN_PROXIMITY     = 0x40  /**< Proximity Sensor (Proximity Loop) */
} cy8ckit_032_button_t;

/** \cond INTERNAL */

// I2C memory offsets for manipulating data on the shield.
#define _CY8CKIT_032_I2C_OFFSET_DAC_VALUE      (0x00)
#define _CY8CKIT_032_I2C_OFFSET_LED_VALUE      (0x04)
#define _CY8CKIT_032_I2C_OFFSET_LED_CONTROL    (0x05)
#define _CY8CKIT_032_I2C_OFFSET_BUTTON_STATE   (0x06)
#define _CY8CKIT_032_I2C_OFFSET_TEMPERATURE    (0x07)
#define _CY8CKIT_032_I2C_OFFSET_HUMIDITY       (0x0B)
#define _CY8CKIT_032_I2C_OFFSET_AMBIENT_LIGHT  (0x0F)
#define _CY8CKIT_032_I2C_OFFSET_POTENTIOMETER  (0x13)

// Utility functions for reading/writing data to the shield.
cy_rslt_t _cy8ckit_032_get_byte(uint8_t offset, uint8_t* value);
cy_rslt_t _cy8ckit_032_set_byte(uint8_t offset, uint8_t value);
cy_rslt_t _cy8ckit_032_get_float(uint8_t offset, float* value);
cy_rslt_t _cy8ckit_032_set_float(uint8_t offset, float value);

/** \endcond */

/**
 * Initializes the I2C interface for communicating with the AFE and OLED
 * components on the board.
 * @param[in] inst    An optional I2C instance to use for communicating with
 * the peripherals on the shield. If NULL, a new instance will be allocated
 * using the CYBSP_I2C_SCL & CYBSP_I2C_SDA pins.
 * @return CY_RSLT_SUCCESS if properly initialized, else an error indicating
 * what went wrong.
 */
cy_rslt_t cy8ckit_032_init(cyhal_i2c_t* inst);

/**
 * Frees up any resources allocated as part of \ref cy8ckit_032_init().
 */
void cy8ckit_032_free(void);

/**
 * Updates the output voltage from the DAC on the shield board.
 * @param[in] volts  Desired DAC voltage in Volts
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_set_dac_voltage(float volts)
{
    return _cy8ckit_032_set_float(_CY8CKIT_032_I2C_OFFSET_DAC_VALUE, volts);
}


/**
 * Gets the current state of the CapSense LEDs on the shield board.
 * @param[out] states  The current state of each of the CLEDs.
 * See \ref cy8ckit_032_cled_t for bitmasks.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_get_cled_states(uint8_t* states)
{
    return _cy8ckit_032_get_byte(_CY8CKIT_032_I2C_OFFSET_LED_VALUE, states);
}


/**
 * Gets the current state of a single CapSense LED on the shield board.
 * @param[in]  cled    The CLED to get the state for
 * @param[out] state   The current state of the requested CLED.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
cy_rslt_t cy8ckit_032_get_cled_state(cy8ckit_032_cled_t cled, bool* state);

/**
 * Sets the current state of the CapSense LEDs on the shield board if it is
 * configured for Software Control (\ref cy8ckit_032_set_led_control()).
 * @param[in] states  The desired state of each of the CLED pins.
 * See \ref cy8ckit_032_cled_t for bitmasks.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_set_cled_states(uint8_t states)
{
    return _cy8ckit_032_set_byte(_CY8CKIT_032_I2C_OFFSET_LED_VALUE, states);
}


/**
 * Sets the current state of the referenced CapSense LED on the shield board
 * if it is configured for Software Control (\ref cy8ckit_032_set_led_control()).
 * @param[in]  cled    The CLED to get the state for
 * @param[in]  state   The desired state of the requested CLED.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
cy_rslt_t cy8ckit_032_set_cled_state(cy8ckit_032_cled_t cled, bool state);

/**
 * Sets the control behavior for the CapSense LEDs.
 * @param[in]  control  The controller for the CLEDs
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_set_led_control(cy8ckit_032_led_ctrl_t control)
{
    return _cy8ckit_032_set_byte(_CY8CKIT_032_I2C_OFFSET_LED_CONTROL, control);
}


/**
 * Gets the current controller mode for the CapSense LEDs.
 * @param[out] control  The controller mode for the CLEDs.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_get_led_control(cy8ckit_032_led_ctrl_t* control)
{
    return _cy8ckit_032_get_byte(_CY8CKIT_032_I2C_OFFSET_LED_CONTROL, (uint8_t*)control);
}


/**
 * Gets the current state of the Buttons on the shield board.
 * @param[out] states  The current state of each of the Buttons.
 * See \ref cy8ckit_032_button_t for bitmasks.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_get_button_states(uint8_t* states)
{
    return _cy8ckit_032_get_byte(_CY8CKIT_032_I2C_OFFSET_BUTTON_STATE, states);
}


/**
 * Gets the current state of a single Button on the shield board.
 * @param[in]  button  The Button to get the state for
 * @param[out] state   The current state of the requested Button.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
cy_rslt_t cy8ckit_032_get_button_state(cy8ckit_032_button_t button, bool* state);

/**
 * Gets the current temperature, in degrees C, from the shield board.
 * @param[out] temperature  The current temperature in degrees C.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_get_temperature_c(float* temperature)
{
    return _cy8ckit_032_get_float(_CY8CKIT_032_I2C_OFFSET_TEMPERATURE, temperature);
}


/**
 * Gets the current humidity, in percent, from the shield board.
 * @param[out] humidity  The current humidity percent.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_get_humidity_percent(float* humidity)
{
    return _cy8ckit_032_get_float(_CY8CKIT_032_I2C_OFFSET_HUMIDITY, humidity);
}


/**
 * Gets the ambient light measurement, in lux, from the shield board.
 * @param[out] light  The current ambient light in lux.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_get_ambient_light_lux(float* light)
{
    return _cy8ckit_032_get_float(_CY8CKIT_032_I2C_OFFSET_AMBIENT_LIGHT, light);
}


/**
 * Gets the current potentiomenter voltage from the shield board.
 * @param[out] voltage  The current potentiometer voltage.
 * @return CY_RSLT_SUCCESS if successful, else an error about
 * what went wrong
 */
static inline cy_rslt_t cy8ckit_032_get_potentiometer_voltage(float* voltage)
{
    return _cy8ckit_032_get_float(_CY8CKIT_032_I2C_OFFSET_POTENTIOMETER, voltage);
}


#if defined(__cplusplus)
}
#endif

/** \} group_board_libs_afe */
