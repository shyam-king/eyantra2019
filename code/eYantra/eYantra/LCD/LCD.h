/*
 * LCD.h
 *
 * Created: 2/1/2020 2:13:22 PM
 *  Author: Shyam
 */ 


#ifndef LCD_H_
#define LCD_H_

class LCD {
	public:

    static void init();
    static void dataWrite( char a);
    static void commandWrite(char a);
    static void setCursor(int a, int b);
    static void display(const char*);
    static void display(int);

    static void clear();
};



#endif /* LCD_H_ */
