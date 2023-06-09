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

typedef struct
{
	volatile uint16_t MODER[2];
	volatile uint16_t OTYPER[2];
	volatile uint16_t OSPEEDR[2];
	volatile uint16_t PUPDR[2];
	volatile uint16_t IDR;
	uint16_t RESERVED1;
	volatile uint16_t ODR[2];
	volatile uint16_t BSRR[2];
	volatile uint16_t LCKR[2];
	volatile uint16_t AFRL[2];
	volatile uint16_t AFRH[2];
	volatile uint16_t BRR[2];
	uint16_t RESERVED2[2];
	volatile uint16_t SECCFGR[2];
} GPIO_Type;

uint32_t wait_milisecond = 1000;
uint32_t sequence_counter = 0;

#define RCC_AHB2ENR *((volatile uint32_t *)(0x40021000 + 0x4C))
#define GPIO_base 0x42020000
#define GPIOA ((GPIO_Type *)(GPIO_base))
#define GPIOB ((GPIO_Type *)(GPIO_base + 0x400))
#define GPIOC ((GPIO_Type *)(GPIO_base + 0x800))

int main(void)
{
	RCC_AHB2ENR |= 7 << 0;

	// PA9 red make all 0
	GPIOA->MODER[1] &= ~(3 << (1 * 2));
	// PB7 blue make all 0
	GPIOB->MODER[0] &= ~(3 << (7 * 2));
	// PB11 pull up make all 0
	GPIOB->MODER[1] &= ~(3 << (3 * 2));
	// PC7 green make all 0
	GPIOC->MODER[0] &= ~(3 << (7 * 2));

	// PA9 red make 01
	GPIOA->MODER[1] |= (1 << (1 * 2));
	// PB7 blue make 01
	GPIOB->MODER[0] |= (1 << (7 * 2));
	// PC7 green make 01
	GPIOC->MODER[0] |= (1 << (7 * 2));
	// PB11 pull up needs to be 00 dont change

	// PB11 make pull up
	GPIOB->PUPDR[1] &= ~(3 << (3 * 2));
	GPIOB->PUPDR[1] |= (1 << (3 * 2));

	// input ground okuduğu zaman doncak
	// input 1 okuduğu zaman

	while (1)
	{
		int index;

		// PA9 red
		GPIOA->ODR[0] |= (1 << 9);
		// PB7 blue
		GPIOB->ODR[0] &= ~(1 << 7);
		// PC7 green
		GPIOC->ODR[0] &= ~(1 << 7);

		for (index = 0; index < wait_milisecond * 333; index++)
			while (!(GPIOB->IDR & 1 << 11))
				;

		// PA9 red
		GPIOA->ODR[0] &= ~(1 << 9);
		// PB7 blue
		GPIOB->ODR[0] |= (1 << 7);
		// PC7 green
		GPIOC->ODR[0] &= ~(1 << 7);

		for (index = 0; index < wait_milisecond * 333; index++)
			while (!(GPIOB->IDR & 1 << 11))
				;

		// PA9 red
		GPIOA->ODR[0] &= ~(1 << 9);
		// PB7 blue
		GPIOB->ODR[0] &= ~(1 << 7);
		// PC7 green
		GPIOC->ODR[0] |= (1 << 7);

		for (index = 0; index < wait_milisecond * 333; index++)
			while (!(GPIOB->IDR & 1 << 11))
				;
				
		__asm volatile("LDR R12, [%0]"
					   :
					   : "r"(&sequence_counter));
		__asm volatile("ADD R12, 1");
		__asm volatile("STR R12, [%0]"
					   :
					   : "r"(&sequence_counter));
	}
}
