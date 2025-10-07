// button_interrupt.c
// Josh Brake
// jbrake@hmc.edu
// 10/31/22
#include <stdio.h>
#include "main.h"

int encoder_count;

int main(void) {

    // Enable button as input
    gpioEnable(GPIO_PORT_A); // enable port A i.e. turn on clk
    // Enable pins as inputs
    pinMode(ENCODERA, GPIO_INPUT);
    pinMode(ENCODERB, GPIO_INPUT);

    // Yes pull downs
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD2, 0b10); // Set PA2 as pull-down
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD1, 0b10); // Set PA1 as pull-down

    // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    // TODO
    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // 2. Configure EXTICR for the input button interrupt
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI2, 0b000); // Select PA2
    // 2. Configure EXTICR for the input button interrupt
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI1, 0b000); // Select PA1

    // Enable interrupts globally
    __enable_irq();

    // TODO: Configure interrupt for falling edge of GPIO pin for button
    // TODO: trigger on rising and falling edge
    // 1. Configure mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(ENCODERA)); // Configure the mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(ENCODERB)); // Configure the mask bit

    // TODO figure out what line my interrupt is on
    // 2. Enable rising edge trigger
    EXTI->RTSR1 |= (1 << gpioPinOffset(ENCODERA));
    EXTI->RTSR1 |= (1 << gpioPinOffset(ENCODERB));

    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(ENCODERA)); // Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(ENCODERB)); // Enable falling edge trigger

    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI2_IRQn);
    NVIC->ISER[0] |= (1 << EXTI1_IRQn); // ?? 

    // Turn on the USART



    while(1){
        compute_velocity();
        delay_millis(TIM2, 1000);
    }

}
// interrupt for A
void EXTI1_IRQHandler(void){ // connected to PA1
    if (EXTI->PR1 & (1 << gpioPinOffset(ENCODERA))) {                                                                                                                                                         & (1 << )){
        EXTI->PR1 |= (1 << gpioPinOffset(ENCODERA)); // fill this in 
        // do the things
        if (digitalRead(ENCODERA) && digitalRead(ENCODERB)) { // rising edge  
            encoder_count--; 
        }
        else if (digitalRead(ENCODERA) && !digitalRead(ENCODERB)){
            encoder_count++;
        }
        else if (!digitalRead(ENCODERA) && digitalRead(ENCODERB)){
            encoder_count++;
        }
        else if (!digitalRead(ENCODERA) && !digitalRead(ENCODERB)){
            encoder_count--;
        } else { // do nothing 
            } 
    }
}

// interrupt for B
void EXTI2_IRQHandler(void){ // connected to PA2
    if (EXTI->PR1 & (1 << gpioPinOffset(ENCODERB))) {                                                                                                                                                         & (1 << )){
        EXTI->PR1 |= (1 << gpioPinOffset(ENCODERB)); // fill this in 
        // do the things
        if (digitalRead(ENCODERA) && digitalRead(ENCODERB)) { // rising edge  
            encoder_count++; 
        }
        else if (digitalRead(ENCODERA) && !digitalRead(ENCODERB)){
            encoder_count--;
        }
        else if (!digitalRead(ENCODERA) && digitalRead(ENCODERB)){
            encoder_count--;
        }
        else if (!digitalRead(ENCODERA) && !digitalRead(ENCODERB)){
            encoder_count++;
        } else { // do nothing 
            } 
    }
}

void compute_velocity(void){
    // do this every ms
    // clear the count
    double rev_s = ((float) encoder_count) / (PPR*4);
    // clear the counter
    encoder_count = 0;
    // call pring velocity
    printf("The motor is going at %.3f revolutions/second\n", rev_s);

}