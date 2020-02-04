/*
 * UART.h
 *
 * Created: 2/1/2020 7:44:52 AM
 *  Author: Shyam
 */

#ifndef UART_H_
#define UART_H_

class Serial {
	public:
	
	static void init();
	static void print(const char *);
	static void println(const char*);
    static void print(int);
	static void print(float);
};


#endif /* UART_H_ */ 