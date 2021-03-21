/*
 * oil.c
 *
 * Created: 7/24/2019 9:17:05 PM
 * Author : HASHITHA KEERTHI
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "i2cmaster.h"
#include "i2c_lcd.h"

int TimerOverflow = 0;
//echo pin pd6

static volatile int pulse = 0;		  // integer to access all though the program
static volatile int i = 0;			  // integer to access all though the program

char numberpressed();
void displayMenu();
void oilVolume();
void gettingOil();
void select();


int main(void)
{
		DDRB = 0xF0;
		DDRA = 0xFF;
		PORTA = 0xFF;
	
	
		int16_t count_a = 0;
		char show_a[16];
		char c;
		int p = 0,q;
		char *msg = "Smart Oil supplier";
		
		int l = strlen(msg);
		
		lcd_init(LCD_BACKLIGHT_ON);
		DDRD = 0b11110011;
		
		PORTA=0xF0;
		_delay_ms(50);

		lcd_init(LCD_BACKLIGHT_ON);

		GICR |= 1<<INT0;
		MCUCR |= 1<<ISC00; 
	
		sei();
		
		lcd_clear();
		lcd_goto_xy(2,0);
		lcd_puts("  Welcome");
		lcd_goto_xy(-1,2);
		lcd_puts("Group No.04");
		lcd_goto_xy(-4,3);
		lcd_puts("Press A to Start");
		
		while(1){
			
			c = numberpressed();
			
			if(c == 'a'){
				displayMenu();
				break;
			}
			
			if((p + 16) < l){
				for(q=0;q<16;q++){
					lcd_goto_xy(q,1);
					lcd_putc(msg[q+p]);
				}
				++p;
				}else{
				p = 0;
			}
			_delay_ms(20);	
		}
		
}

void gettingOil(){
	
	char str[20];
	char c;
	int i = 0,num,j;
	int t;
	
	lcd_clear();
	lcd_goto_xy(1,0);
	lcd_puts(" Getting Oil");
	lcd_goto_xy(1,1);
	lcd_puts("Enter Volume");
	
	lcd_goto_xy(1,2);
	while(1){
		c = numberpressed();
		if(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0'){
			lcd_goto_xy((1+i),2);
			lcd_putc(c);
			str[i] = c;
			++i;
		}else if(c == '*'){
			num = atoi(str);
			PORTA = 0x02;
			t = (int) num/31;
			for(j=0;j<t;j++){
				_delay_ms(1000);
			}
			PORTA = 0x00;
			break;
		}
		_delay_ms(20);
	}
	
	displayMenu();
	
}

void displayMenu(){
	char c;
	int flag = 1;
	char *msg1 = "Select option and press B button";
	int l1 = strlen(msg1);
	int i=0,j;
	lcd_clear();
	lcd_goto_xy(1,0);
	lcd_puts(" C oil ");
	lcd_goto_xy(1,1);
	lcd_puts(" Dencity ");
	lcd_goto_xy(-3,2);
	lcd_puts(" ________");
	lcd_goto_xy(13,1);
	lcd_putc('<');
	
	while(1){
		c = numberpressed();
		if(c == '2'){
			flag = 1;
			lcd_goto_xy(13,1);
			lcd_putc('<');
			lcd_goto_xy(9,2);
			lcd_putc(' ');
			}else if(c == '8'){
			flag = 2;
			lcd_goto_xy(9,2);
			lcd_putc('<');
			lcd_goto_xy(13,1);
			lcd_putc(' ');
			}else if(c == '*'){
				if(flag == 1){
					lcd_clear();
					oilVolume();
				}else if(flag == 2){
					gettingOil();
				}
			}
		
		if((i + 16) < l1){
			for(j=0;j<16;j++){
				lcd_goto_xy(j-4,3);
				lcd_putc(msg1[j+i]);
			}
			++i;
			}else{
			i = 0;
		}
		_delay_ms(20);
	}
}

void oilVolume(){
	
			char show_a[16];
			int16_t count_a = 0;
			char c;
			char *str = "Press Stop button to Back";
			int l = strlen(str);
			int i = 0,j;
			
			
			while(1){
			
			c = numberpressed();
			
			if(c == '#'){
				displayMenu();
			}
			
			PORTD |= 1<<PIND0;
			_delay_us(50);
			PORTD &= ~(1<<PIND0);
			count_a = pulse/58;
			
			itoa(count_a,show_a,10);
			
			lcd_goto_xy(1,0);
			lcd_puts("oil volume");
			lcd_goto_xy(4,1);
			itoa((36-count_a)*600,show_a,10);//r=14cm
			lcd_puts(show_a);
			lcd_puts(" ");
			lcd_goto_xy(10,1);
			lcd_puts("ml");
			lcd_goto_xy(1,1);
			
			if((i + 16) < l){
				for(j=0;j<16;j++){
					lcd_goto_xy(j-3,3);
					lcd_putc(str[j+i]);
				}
				++i;
				}else{
				i = 0;
			}
			}
}



char numberpressed()
{
	PORTB = 0b10000000;
	if(PINB & (1<<PB0))
	{
		return 'a';
	}
	if(PINB & (1<<PB1))
	{
		return '3';
	}
	if(PINB & (1<<PB2))
	{
		return '2';
	}
	if(PINB & (1<<PB3))
	{
		return '1';
	}
	
	PORTB = 0b01000000;
	if(PINB & (1<<PB0))
	{
		return 'b';
	}
	if(PINB & (1<<PB1))
	{
		return '6';
	}
	if(PINB & (1<<PB2))
	{
		return '5';
	}
	if(PINB & (1<<PB3))
	{
		return '4';
	}
	PORTB = 0b00100000;
	if(PINB & (1<<PB0))
	{ 
		return 'c';
	}
	if(PINB & (1<<PB1))
	{
		return '9';
	}
	if(PINB & (1<<PB2))
	{
		
		return '8';
	}
	if(PINB & (1<<PB3))
	{
		return '7';
	}
	PORTB = 0b00010000;
	if(PINB & (1<<PB0))
	{
		return 'd';
	}
	if(PINB & (1<<PB1))
	{
		return '#';
	}
	if(PINB & (1<<PB2))
	{
		return '0';
	}
	if(PINB & (1<<PB3))
	{
		return '*';
	}
	return 'N';
}


ISR(INT0_vect)
{
  if(i == 1)
  {
    TCCR1B = 0;
    pulse = TCNT1;
    TCNT1 = 0;
    i = 0;
  }

  if(i==0)
  {
    TCCR1B |= 1<<CS10;
    i = 1;
  }
}



	



