/*
 * LCD.cpp
 *
 * Created: 2/1/2020 2:13:13 PM
 *  Author: Shyam
 */ 

#include "LCD.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define control_bus PORTK
#define controlbus_direction DDRK
#define data_bus PORTE
#define databus_direction DDRE

#define rs 0
#define en 1
#define d4 2
#define d5 3
#define d6 4
#define d7 5

void LCD::init() {
    controlbus_direction |= ((1<<rs)|(1<<en));
	databus_direction |= ((1<<d7)|(1<<d6)|(1<<d5)|(1<<d4));
	_delay_ms(2);
	commandWrite(0x01); // clear display
	commandWrite(0x02); // back to home
	commandWrite(0x28); // 4bit,2line,5x7 pixel
	commandWrite(0x06); // entry mode,cursor increments by cursor shift
	commandWrite(0x0c); // display ON,cursor OFF
	commandWrite(0x80); // force cursor to begin at line1
}

void LCD::setCursor(int a, int b) {
	int i=0;
	switch(b){
		case 0: commandWrite(0x80);break;
		case 1: commandWrite(0xC0);break;
		case 2: commandWrite(0x94);break;
	    case 3: commandWrite(0xd4);break;
    }

	for(i=0;i<a;i++)
	commandWrite(0x14);
}

void LCD::commandWrite( char a) {
	if(a & 0x80) data_bus|=(1<<d7); else data_bus&= ~(1<<d7);
	if(a & 0x40) data_bus|=(1<<d6); else data_bus&= ~(1<<d6);
	if(a & 0x20) data_bus|=(1<<d5); else data_bus&= ~(1<<d5);
	if(a & 0x10) data_bus|=(1<<d4); else data_bus&= ~(1<<d4);
	control_bus &=~(1<<rs);control_bus |=(1<<en);
	_delay_ms(2);
	control_bus &=~(1<<en);
	
	_delay_ms(2);
	
	if(a & 0x08) data_bus|=(1<<d7); else data_bus&= ~(1<<d7);
	if(a & 0x04) data_bus|=(1<<d6); else data_bus&= ~(1<<d6);
	if(a & 0x02) data_bus|=(1<<d5); else data_bus&= ~(1<<d5);
	if(a & 0x01) data_bus|=(1<<d4); else data_bus&= ~(1<<d4);
	control_bus &=~(1<<rs);control_bus |=(1<<en);
	_delay_ms(2);
	control_bus &=~(1<<en);
	
	_delay_ms(2);
}

void LCD::display(const char *p) {
	while (*p!='\0') {
		dataWrite(*p);
		p++;
	}
}

void LCD::display(int v) {
    char temp[15];
    sprintf(temp, "%d", v);
    display(temp);
}


void LCD::dataWrite( char a)
{
	
	if(a & 0x80) data_bus|=(1<<d7); else data_bus&= ~(1<<d7);
	if(a & 0x40) data_bus|=(1<<d6); else data_bus&= ~(1<<d6);
	if(a & 0x20) data_bus|=(1<<d5); else data_bus&= ~(1<<d5);
	if(a & 0x10) data_bus|=(1<<d4); else data_bus&= ~(1<<d4);
	control_bus |=(1<<rs)|(1<<en);
	_delay_ms(2);
	control_bus &=~(1<<en);
	_delay_ms(2);
	
	if(a & 0x08) data_bus|=(1<<d7); else data_bus&= ~(1<<d7);
	if(a & 0x04) data_bus|=(1<<d6); else data_bus&= ~(1<<d6);
	if(a & 0x02) data_bus|=(1<<d5); else data_bus&= ~(1<<d5);
	if(a & 0x01) data_bus|=(1<<d4); else data_bus&= ~(1<<d4);
	control_bus |=(1<<rs)|(1<<en);
	_delay_ms(2);
	control_bus &=~(1<<en);
	_delay_ms(2);
}

void LCD::clear() {
	commandWrite(0x01);
    commandWrite(0x02);
}