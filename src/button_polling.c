// button_polling.c
// Josh Brake
// jbrake@hmc.edu
// 10/31/22

/*
  This program polls the user button on the Nucleo-L432KC board and has a
  delay within the main loop to simulate the problems with polling for 
  catching events.
*/



#include "main.h"

int main(void) {
    // Enable LED as output
    gpioEnable(GPIO_PORT_A);
    pinMode(ENCODERA, GPIO_INPUT);
    pinMode(ENCODERB, GPIO_INPUT);

   
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD2, 0b10); // Set PA2 as pull-down
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD1, 0b10); // Set PA1 as pull-down

    // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    double rev_s;
    int encoder_count;
    int volatile cur_encA_state = digitalRead(ENCODERA);
    int volatile cur_encB_state = digitalRead(ENCODERB);

    int volatile prev_encA_state = cur_encA_state;
    int volatile prev_encB_state = cur_encB_state;

    while(1){
        prev_encA_state = cur_encA_state;
        prev_encB_state = cur_encB_state;
        cur_encA_state = digitalRead(ENCODERA);
        cur_encB_state = digitalRead(ENCODERB);

        // if state change on A 
        if (cur_encA_state != prev_encA_state){
            
            if (cur_encA_state && cur_encB_state) { // rising edge  
                encoder_count--; 
            }
            else if (cur_encA_state && !cur_encB_state){
                encoder_count++;
            }
            else if (!cur_encA_state && cur_encB_state){
                encoder_count++;
            }
            else if (!cur_encA_state && !cur_encB_state){
                encoder_count--;
            }
        }

        // if state change on B
        if (cur_encB_state != prev_encB_state){
            if (cur_encA_state && cur_encB_state) { // rising edge  
                encoder_count++; 
            }
            else if (cur_encA_state && !cur_encB_state){
                encoder_count--;
            }
            else if (!cur_encA_state && cur_encB_state){
                encoder_count--;
            }
            else if (!cur_encA_state && !cur_encB_state){
                encoder_count++;
            }
        }
        
    }
    delay_millis(DELAY_TIM, 1000);
    rev_s = ((float) encoder_count) / (PPR*4);
    encoder_count = 0;
    printf("The motor is going at %.3f revolutions/second\n", rev_s);
}
