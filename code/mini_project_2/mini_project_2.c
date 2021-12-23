/*
 * mini_project_2.c
 *
 *  Created on: Sep 20, 2021
 *      Author: Shawky Abo El_Ela
 */



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char sec1 = 0;
unsigned char sec2 = 0;
unsigned char min1 = 0;
unsigned char min2 = 0;
unsigned char hour1 = 0;
unsigned char hour2 = 0;



void TIMER1_init(void){
	TCNT1 = 0;
	OCR1A = 1000; // after 1000 counts will reach 1 second
	TIMSK |= (1<< OCIE1A); // enable interrupt 1 A
	/*
	 * Non PWM
	 * pre-scaling = 1024;
	 * normal port operation
	 * CTC mode
	 */
	TCCR1A  = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<< CS10) | (1<<CS12);
}

void INT0_init(void){
	DDRD &= ~(1<< PD2); //input
	PORTC |= (1<< PD2);  // internal pull up
	MCUCR |= (0<<ISC00) | (1 <<ISC01); ; // falling edge
	GICR |= (1<<INT0);
}

void INT1_init(void){
	DDRD &= ~(1<< PD3); //input ->pull down
	MCUCR |= (1<<ISC00) | (1 <<ISC01); // raising edge
	GICR |= (1<<INT1);
}

void INT2_init(void){
	DDRB &= ~(1<<PB2); //input
	PORTB |= (1<<PB2);// internal pull up
	MCUCSR |= (0<< ISC2);
	GICR |= (1<<INT2);

}
ISR(TIMER1_COMPA_vect){

		sec1++;
		if(sec1==10){
			sec2++;
			sec1 = 0;
		}
		if(sec2 == 6){
			min1++;
			sec2=0;
			sec1=0;
			if (min1==10){
				min2++;
				min1 = 0;
			}

		}
		if(min2==6){
			hour1++;
			min1=0;
			min2=0;
			if(hour1==10){
				hour2++;
				hour1 = 0;
			}

		}

}

ISR(INT0_vect){
	sec1 = 0;
	sec2 = 0;
	min1 = 0;
	min2 = 0;
	hour1= 0;
	hour2= 0;
}

ISR(INT1_vect){
	TCCR1A =0;
	TCCR1B =0;
}
ISR(INT2_vect){
	TCCR1A  = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<< CS10) | (1<<CS12);
}
int main(){
	DDRC |= 0x0F; // output pins
    PORTC &= 0xF0;
    DDRA |= 0x3F;  // output to enable/ disable each segment
    PORTA |=  0x3F;  // Initially the all on

    SREG |= (1<<7);

    INT0_init();
    INT1_init();
    INT2_init();
    TIMER1_init();

	while(1){

		PORTA =(1<<PA0); // set PA0 and clear the rest of the them
		PORTC = (PORTC & 0xF0) | (sec1 & 0x0F);
		_delay_us(10);
		PORTA =(1<<PA1);
		PORTC = (PORTC & 0xF0) | (sec2 & 0x0F);
		_delay_us(10);
		PORTA =(1<<PA2);
		PORTC = (PORTC & 0xF0) | (min1 & 0x0F);
		_delay_us(10);
		PORTA =(1<<PA3);
		PORTC = (PORTC & 0xF0) | (min2 & 0x0F);
		_delay_us(10);
		PORTA =(1<<PA4);
		PORTC = (PORTC & 0xF0) | (hour1 & 0x0F);
		_delay_us(10);
		PORTA =(1<<PA5);
		PORTC = (PORTC & 0xF0) | (hour2 & 0x0F);
		_delay_us(10);





	}
}
