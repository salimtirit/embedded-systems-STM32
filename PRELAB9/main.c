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
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
	volatile uint32_t BRR;
	uint32_t RESERVED;
	volatile uint32_t SECCFGR;
} GPIO_TypeDef;


typedef struct
{
	volatile uint32_t ISR;
	volatile uint32_t IER;
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CFGR2;
	volatile uint32_t SMPR1;
	volatile uint32_t SMPR2;
	uint32_t RESERVED1;
	volatile uint32_t TR1;
	volatile uint32_t TR2;
	volatile uint32_t TR3;
	uint32_t RESERVED2;
	volatile uint32_t SQR1;
	volatile uint32_t SQR2;
	volatile uint32_t SQR3;
	volatile uint32_t SQR4;
	volatile uint32_t DR;
	uint32_t RESERVED3[2];
	volatile uint32_t JSQR;
	uint32_t RESERVED4[4]; //50
	volatile uint32_t OFR1;
	volatile uint32_t OFR2;
	volatile uint32_t OFR3;
	volatile uint32_t OFR4;
	uint32_t RESERVED5[4];
	volatile uint32_t JDR1;
	volatile uint32_t JDR2;
	volatile uint32_t JDR3;
	volatile uint32_t JDR4;
	uint32_t RESERVED6[4];
	volatile uint32_t AWD2CR;
	volatile uint32_t AWD3CR;
	uint32_t RESERVED7[2];
	volatile uint32_t DIFSEL;
	volatile uint32_t CALFACT;
} ADC_TypeDef;

#define RCC_AHB2ENR *((volatile uint32_t *)(0x40021000 + 0x4C))
#define RCC_APB1ENR1 *((volatile uint32_t *)(0x40021000 + 0x58))
#define RCC_CCIPR1 *((volatile uint32_t *)(0x40021000 + 0x088))
#define NVIC_ISER1 *((volatile uint32_t *) (0xE000E100 + 0x4))

#define GPIOA ((GPIO_TypeDef *)(0x42020000))
#define GPIOB ((GPIO_TypeDef *)(0x42020000 + 0x400))
#define GPIOC ((GPIO_TypeDef *)(0x42020000 + 0x800))
#define ADC1 ((ADC_TypeDef *)(0x42020000 + 0x8000))

void ADC1_2_IRQHandler() {
	uint32_t value = ADC1->DR; //4034 0
	if(value > 4031*0.75){
		// R G B on
		GPIOA->ODR |= 1 << 9;
		GPIOB->ODR |= 1 << 7;
		GPIOC->ODR |= 1 << 7;

	} else if(value > 4031*0.5){
		// G B on
		GPIOA->ODR &= ~(1 << 9);
		GPIOB->ODR |= 1 << 7;
		GPIOC->ODR |= 1 << 7;

	} else if(value > 4031*0.25){
		// B on
		GPIOA->ODR &= ~(1 << 9);
		GPIOB->ODR |= 1 << 7;
		GPIOC->ODR &= ~(1 << 7);
	} else {
		// all off
		GPIOA->ODR &= ~(1 << 9);
		GPIOB->ODR &= ~(1 << 7);
		GPIOC->ODR &= ~(1 << 7);
	}
	ADC1->CR |= (0x01 << 2);

}


int main(void) {
	//Enable Clock for GPIO
	RCC_AHB2ENR |= (0x07 << 0); // gpio a b c enable

	// PA9 red make all 0
	GPIOA->MODER &= ~(3 << (9 * 2));
	// PA9 red make 01
	GPIOA->MODER |= (1 << (9 * 2));

	// PB7 blue make all 0
	GPIOB->MODER &= ~(3 << (7 * 2));
	// PB7 blue make 01
	GPIOB->MODER |= (1 << (7 * 2));

	// PC7 green make all 0
	GPIOC->MODER &= ~(3 << (7 * 2));
	// PC7 green make 01
	GPIOC->MODER |= (1 << (7 * 2));

	//Enable Clock for ADC
	RCC_AHB2ENR |= (1 << 13);

	//Select ADC clock as System clock
	RCC_CCIPR1 |= (0x03 << 28);

	//Change Pin Mode to Analog
	GPIOC->MODER |= (0x03 << 0);

	//Change Pin Pull/Down to no pull-up no pull-down
	GPIOC->PUPDR &= ~(0x03 << 0);

	//Change Regular channel sequence length to 1 conversion
	ADC1->SQR1 &= ~(0x0F << 0);

	//Add to channel to first sequence
	ADC1->SQR1 |= 0x01 << 6;

	//Disable Deep-power-down for ADC
	ADC1->CR &= ~(0x01 << 29);

	//Enable ADC Voltage regulator
	ADC1->CR |= (0x01 << 28);

	//Configure for Single conversion mode
	ADC1->CFGR &= ~(0x01 << 13);

	//Enable ADC
	ADC1->CR |= (0x01 << 0);

	//Wait ADC is enabled
	while(!(ADC1->ISR & 0x01)){}

	//Enable interrupt for end of regular conversion
	ADC1->IER |= (0x01 << 2);

	NVIC_ISER1 |= (0x01 << 5);

	__asm volatile( // enable all interrupts that are configured
		"mov r0, #0 \n\t"
		"msr primask, r0 \n\t");


	//Start regular conversion of ADC
	ADC1->CR |= (0x01 << 2);

	//Write program according to problem description
	while(1){
	}
}
