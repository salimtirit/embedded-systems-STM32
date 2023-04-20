/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>

uint32_t wait_millisecond = 1000;
uint32_t wait_counter = 0;

//Define Registers
#define RCC_AHB2ENR  *((volatile uint32_t *) 0X4002 104C )
#define MODER *((volatile uint32_t *) 0x4202 0000)
#define ODR *((volatile uint32_t *) 0x4202 0014)

int main(void) {
	//Enable Clock for GPIO
	RCC_AHB2ENR  |= 1 << 0;

	//Configure Pin as General purpose output mode
	MODER &= ~(1 << 19);

	while(1) {
		int index;

		//Turn On LED
		ODR |= (1 << 9);

		for(index=0;index<wait_millisecond*333;index++);
		wait_counter = wait_counter + 1;

		//Turn Off LED
		ODR &= ~(1 << 9);

		for(index=0;index<wait_millisecond*333;index++);
		wait_counter = wait_counter + 1;
	}
}