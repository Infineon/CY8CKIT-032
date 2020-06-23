/******************************************************************************
 * \file cy8ckit_032.c
 *
 * Description: This file contains the implementation for the CY8CKIT-032
 * Analog Front End shield.
 *
 *******************************************************************************
 * \copyright
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

#include "cy8ckit_032.h"
#include "cyhal_i2c.h"

#define AFE_I2C_ADDRESS             (0x42)

static cyhal_i2c_t i2c;
static cyhal_i2c_t *i2c_ptr = NULL;


/*******************************************************************************
 * Utility functions for reading data from the shield
 *******************************************************************************/
cy_rslt_t _cy8ckit_032_get_byte(uint8_t offset, uint8_t *value)
{
    cy_rslt_t rslt = cyhal_i2c_master_write(i2c_ptr, AFE_I2C_ADDRESS, &offset, 1, 0, true);
    if (CY_RSLT_SUCCESS == rslt)
        rslt = cyhal_i2c_master_read(i2c_ptr, AFE_I2C_ADDRESS, value, 1, 0, true);
    return rslt;
}


/*******************************************************************************
 * Utility functions for writing data to the shield
 *******************************************************************************/
cy_rslt_t _cy8ckit_032_set_byte(uint8_t offset, uint8_t value)
{
    uint8_t buff[2] = { offset, value };
    return cyhal_i2c_master_write(i2c_ptr, AFE_I2C_ADDRESS, buff, 2, 0, true);
}


/*******************************************************************************
 * Utility functions for reading data from the shield
 *******************************************************************************/
cy_rslt_t _cy8ckit_032_get_float(uint8_t offset, float *value)
{
    cy_rslt_t rslt = cyhal_i2c_master_write(i2c_ptr, AFE_I2C_ADDRESS, &offset, 1, 0, true);
    if (CY_RSLT_SUCCESS == rslt)
        rslt = cyhal_i2c_master_read(i2c_ptr, AFE_I2C_ADDRESS, (uint8_t*)value, 4, 0, true);
    return rslt;
}


/*******************************************************************************
 * Utility functions for writing data to the shield
 *******************************************************************************/
cy_rslt_t _cy8ckit_032_set_float(uint8_t offset, float value)
{
    uint8_t buff[5] = {
        offset,
        (uint8_t)(value),
        (uint8_t)(((uint32_t)value) >> 8),
        (uint8_t)(((uint32_t)value) >> 16),
        (uint8_t)(((uint32_t)value) >> 24)
    };
    return cyhal_i2c_master_write(i2c_ptr, AFE_I2C_ADDRESS, buff, 5, 0, true);
}


/*******************************************************************************
 * Shield initialization function
 *******************************************************************************/
cy_rslt_t cy8ckit_032_init(cyhal_i2c_t *inst)
{
    cy_rslt_t rslt = CY_RSLT_SUCCESS;

    if (NULL == inst)
    {
        /* Configuration to initialize the I2C block */
        static const cyhal_i2c_cfg_t I2C_CONFIG = {
            .is_slave = false,
            .address = 0,
            .frequencyhal_hz = 400000
        };

        rslt = cyhal_i2c_init(&i2c, CY8CKIT_032_PIN_I2C_SDA, CY8CKIT_032_PIN_I2C_SCL, NULL);
        if (CY_RSLT_SUCCESS == rslt)
        {
            i2c_ptr = &i2c;
            rslt = cyhal_i2c_configure(&i2c, &I2C_CONFIG);
        }
    }
    else
    {
        i2c_ptr = inst;
    }

    if (CY_RSLT_SUCCESS == rslt)
        mtb_ssd1306_init_i2c(i2c_ptr);

    if (CY_RSLT_SUCCESS != rslt)
        cy8ckit_032_free();

    return rslt;
}


/*******************************************************************************
 * Gets the current state of a single CapSense LED
 *******************************************************************************/
cy_rslt_t cy8ckit_032_get_cled_state(cy8ckit_032_cled_t cled, bool *state)
{
    uint8_t states;
    cy_rslt_t rslt = cy8ckit_032_get_cled_states(&states);
    if (CY_RSLT_SUCCESS == rslt)
        *state = ((states & cled) > 0);
    return rslt;
}


/*******************************************************************************
 * Sets the current state of the CapSense LEDs
 *******************************************************************************/
cy_rslt_t cy8ckit_032_set_cled_state(cy8ckit_032_cled_t cled, bool state)
{
    uint8_t states;
    cy_rslt_t rslt = cy8ckit_032_get_cled_states(&states);

    if (CY_RSLT_SUCCESS == rslt)
    {
        if (state)
            states |= cled;
        else
            states &= !cled;
        _cy8ckit_032_set_byte(_CY8CKIT_032_I2C_OFFSET_LED_VALUE, states);
    }

    return rslt;
}


/*******************************************************************************
 * Gets the current state of a single Button on the shield board.
 *******************************************************************************/
cy_rslt_t cy8ckit_032_get_button_state(cy8ckit_032_button_t button, bool *state)
{
    uint8_t states;
    cy_rslt_t rslt = cy8ckit_032_get_button_states(&states);
    if (CY_RSLT_SUCCESS == rslt)
        *state = ((states & button) > 0);
    return rslt;
}


/*******************************************************************************
 * Frees up any resources allocated as part of the initialization
 *******************************************************************************/
void cy8ckit_032_free()
{
    mtb_ssd1306_free();
    if (i2c_ptr == &i2c)
        cyhal_i2c_free(i2c_ptr);
    i2c_ptr = NULL;
}
