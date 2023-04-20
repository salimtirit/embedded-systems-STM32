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
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
	volatile uint32_t BRR;
	uint32_t RESERVED;
	volatile uint32_t SECCFGR;
} GPIO_TypeDef;

typedef struct
{
	volatile uint32_t CR1; // 0x00
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR; // 0x10
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER; // 0x20
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR; // 0x30
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4; // 0x40
	uint32_t reserved1;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR1; // 0x50
	uint32_t reserved2;
	uint32_t reserved3;
	uint32_t reserved4;
	volatile uint32_t OR2; // 0x60
	uint32_t reserved5;
} TIM_TypeDef;

#define RCC_AHB2ENR *((volatile uint32_t *)(0x40021000 + 0x4C))
#define RCC_APB1ENR1 *((volatile uint32_t *)(0x40021000 + 0x58))

#define GPIOA ((GPIO_TypeDef *)(0x42020000))
// #define GPIOB ((GPIO_TypeDef *)(0x42020000 + 0x400))
// #define GPIOC ((GPIO_TypeDef *)(0x42020000 + 0x800))
#define GPIOD ((GPIO_TypeDef *)(0x42020000 + 0xC00))
// #define GPIOE ((GPIO_TypeDef *)(0x42020000 + 0x1000))
// #define GPIOF ((GPIO_TypeDef *)(0x42020000 + 0x1400))

#define TIM2 ((TIM_TypeDef *)0x40000000)
#define TIM3 ((TIM_TypeDef *)0x40000400)
#define TIM4 ((TIM_TypeDef *)0x40000800)
#define TIM5 ((TIM_TypeDef *)0x40000C00)

#define NVIC_ISER1 *((volatile uint32_t *)(0xE000E100 + 0x04))

// PD7 blue
// PD6 green
// PD5 red

uint32_t state;
uint32_t i = 0;


// red
void TIM2_IRQHandler(void)
{
	TIM2->SR = 0;
	if (state & 0x4)
	{
		if (GPIOD->ODR & 1 << 5)
		{
			GPIOD->ODR &= ~(1 << 5);
		}
		else
		{
			GPIOD->ODR |= (1 << 5);
		}
	}
	else
	{
		GPIOD->ODR |= (1 << 5);
	}
}

// green
void TIM3_IRQHandler(void)
{
	TIM3->SR = 0;
	if (state & 0x2)
	{
		if (GPIOD->ODR & 1 << 6)
		{
			GPIOD->ODR &= ~(1 << 6);
		}
		else
		{
			GPIOD->ODR |= (1 << 6);
		}
	}
	else
	{
		GPIOD->ODR |= (1 << 6);
	}
}

// blue
void TIM4_IRQHandler(void)
{
	TIM4->SR = 0;
	if (state & 0x1)
	{
		if (GPIOD->ODR & 1 << 7)
		{
			GPIOD->ODR &= ~(1 << 7);
		}
		else
		{
			GPIOD->ODR |= (1 << 7);
		}
	}
	else
	{
		GPIOD->ODR |= (1 << 7);
	}
}

// general
void TIM5_IRQHandler(void)
{
	TIM5->SR = 0;
	uint8_t rgb[8] = {0x7, 0x3, 0x5, 0x1, 0x6, 0x2, 0x4, 0x0};
	// White (all) - Cyan (blue-green) - Magenta (blue-red) - Blue - Yellow(red-green) - Green - Red - Black (none)

	state = rgb[i];
	if (i == 7)
	{
		i = 0;
	}else{
		i+=1;
	}

}

void init_TIM2()
{
	RCC_APB1ENR1 |= 1 << 0; // enable tim2
	TIM2->PSC = 4000 - 1;	// Configure prescaler to increase the CNT register for every 1 millisecond TIM2
	TIM2->CR1 &= ~(1 << 1); // OVF will generate an event
	// TIM6 Interrupt Initialization
	TIM2->ARR = 9;		  // red 100Hz
	TIM2->SR = 0;		  // red
	TIM2->DIER |= (0x01); // Enable update interrupt TIM2
	NVIC_ISER1 |= (1 << 13);
	TIM2->CR1 |= (1); // Counter Enable
}

void init_TIM3()
{
	RCC_APB1ENR1 |= 1 << 1; // enable tim3
	TIM3->PSC = 4000 - 1;	// Configure prescaler to increase the CNT register for every 1 millisecond TIM3
	TIM3->CR1 &= ~(1 << 1); // OVF will generate an event
	// TIM6 Interrupt Initialization
	TIM3->ARR = 4;		  // green 200Hz
	TIM3->SR = 0;		  // green
	TIM3->DIER |= (0x01); // Enable update interrupt TIM3
	NVIC_ISER1 |= (1 << 14);
	TIM3->CR1 |= (1); // Counter Enable
}

void init_TIM4()
{
	RCC_APB1ENR1 |= 1 << 2; // enable tim4
	TIM4->PSC = 4000 - 1;	// Configure prescaler to increase the CNT register for every 1 millisecond TIM4
	TIM4->CR1 &= ~(1 << 1); // OVF will generate an event
	// TIM6 Interrupt Initialization
	TIM4->ARR = 2;		  // blue 400Hz
	TIM4->SR = 0;		  // blue
	TIM4->DIER |= (0x01); // Enable update interrupt TIM4
	NVIC_ISER1 |= (1 << 15);
	TIM4->CR1 |= (1); // Counter Enable
}

void init_TIM5()
{
	RCC_APB1ENR1 |= 1 << 3; // enable tim5
	TIM5->PSC = 4000 - 1;	// Configure prescaler to increase the CNT register for every 1 millisecond TIM5
	TIM5->CR1 &= ~(1 << 1); // OVF will generate an event
	// TIM6 Interrupt Initialization
	TIM5->ARR = 999;	  // general 1Hz
	TIM5->SR = 0;		  // general
	TIM5->DIER |= (0x01); // Enable update interrupt TIM5
	NVIC_ISER1 |= (1 << 16);
	TIM5->CR1 |= (1); // Counter Enable
}

int main(void)
{
	RCC_AHB2ENR |= 1 << 0; // 1 << 3; // gpioden
	RCC_AHB2ENR |= 1 << 3; // 1 << 3; // gpioden

	// red on board led output
	GPIOA->MODER &= ~(0x03 << (9 * 2));
	GPIOA->MODER |= (0x01 << (9 * 2));

	// red led output
	GPIOD->MODER &= ~(0x03 << (5 * 2));
	GPIOD->MODER |= (0x01 << (5 * 2));

	// green led output
	GPIOD->MODER &= ~(0x03 << (6 * 2));
	GPIOD->MODER |= (0x01 << (6 * 2));

	// blue led output
	GPIOD->MODER &= ~(0x03 << (7 * 2));
	GPIOD->MODER |= (0x01 << (7 * 2));

	TIM2->EGR |= (0x01); // Generate update interrupt when CNT is reinitialized TIM2
	TIM3->EGR |= (0x01); // Generate update interrupt when CNT is reinitialized TIM3
	TIM4->EGR |= (0x01); // Generate update interrupt when CNT is reinitialized TIM4
	TIM5->EGR |= (0x01); // Generate update interrupt when CNT is reinitialized TIM5

	__asm volatile( // enable all interrupts that are configured
		"mov r0, #0 \n\t"
		"msr primask, r0 \n\t");

	init_TIM2();
	init_TIM3();
	init_TIM4();
	init_TIM5();

	GPIOA->ODR &= ~(1 << 9);

	GPIOD->ODR = 1;

}