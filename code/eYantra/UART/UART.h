/*
 * UART.h
 *
 * Created: 13-Jan-20 1:00:05 AM
 *  Author: Shyam Murugan
 */ 


#ifndef UART_H_
#define UART_H_

class Serial {
	public:
	
	static void begin();
	static void print(const char *);
	static void println(const char*);
};



#endif /* UART_H_ */