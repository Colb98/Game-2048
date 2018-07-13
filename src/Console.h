#ifndef CONSOLE_H
#define CONSOLE_H

/* Includes */
#include <Windows.h>
#include <iostream>

/* Classes */
class Console {
public:
	void setColor(unsigned short color) {
		HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hcon, color);
	}
	void clearscr() {
		system("CLS");
	}
private:

};

void ShowConsoleCursor(bool showFlag);


#endif