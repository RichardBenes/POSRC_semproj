/*
 * Copyright 2016-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    POSRC_semproj.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_port.h"

#include "FreeRTOS.h"
#include "task.h"

/* TODO: insert other definitions and declarations here. */

void vGreenTask(void* params);
void vBlueTask(void* params);

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortD);

    PORT_SetPinMux(PORTB, 19U, kPORT_MuxAsGpio); /* Green LED */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAsGpio); /* Red LED */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAsGpio); /* Blue LED */

    gpio_pin_config_t outConfig = {
		.pinDirection = kGPIO_DigitalOutput,
		.outputLogic = 1U /* Turn those LEDs off at startup */
    };

    GPIO_PinInit(GPIOD, 1U, &outConfig); /* Blue LED */
    GPIO_PinInit(GPIOB, 19U, &outConfig); /* Green LED */
    GPIO_PinInit(GPIOB, 18U, &outConfig); /* Red LED */

    TaskHandle_t greenTask1;
    TaskHandle_t blueTask1;

    xTaskCreate(vGreenTask, "green-task-1", 32U, NULL, 0, &greenTask1);
    xTaskCreate(vBlueTask, "blue-task-1", 32U, NULL, 0, &blueTask1);

    vTaskStartScheduler();

    return 0 ;
}

/* TODO: pass the LED color as an argument */
void vGreenTask(void* param) {

	TickType_t halfPeriod = 1500U;

	for (;;) {
		GPIO_WritePinOutput(GPIOD, 1U, 0U); /* Turn blue LED on */
		vTaskDelay(halfPeriod);
		GPIO_WritePinOutput(GPIOD, 1U, 1U); /* Turn blue LED off */
		vTaskDelay(halfPeriod);
	}

	for (;;) {}
}

void vBlueTask(void* param) {

	TickType_t halfPeriod = 50U;

	for (;;) {
		GPIO_WritePinOutput(GPIOB, 19U, 0U); /* Turn green LED on */
		vTaskDelay(halfPeriod);
		GPIO_WritePinOutput(GPIOB, 19U, 1U); /* Turn green LED off */
		vTaskDelay(halfPeriod);
	}

	for (;;) {}
}
