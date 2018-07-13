#pragma once
#define RATIO 90
#define DISTANCEX 12
#define DISTANCEY 5
#define STARTX 12
#define STARTY 6
#define LOGOX 39
#define LOGOY 3
#define DELAYMOVING 100
#define DEFAULT_SQUARE 4
#define DATAFILE "data.bin"
#define HSFILE "hs.bin"

struct IntArray2D {
	int n;
	int score = 0;
	int highscore = 0;
	int *data = NULL;
	int *operator[](int idx) {
		if (idx < 0 || idx >= n) idx = 0;
		return data + n*idx;
	}
};


typedef IntArray2D Array;

bool intInit(Array &a, int n);
void printTable(Array &a);
void genElement(Array &a);
bool isFull(Array a);
void GotoXY(int x, int y);
bool moveDown(Array &a);
bool moveUp(Array &a);
bool moveLeft(Array &a);
bool moveRight(Array &a);
void drawTile(int val, int n, int x, int y, int dx, int dy); //dx and dy is use for moving
int gameFunc(Array &data, bool &gen);
void drawBoard(Array arr, int n);
DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName);
int menu(Array &data, bool *music);
void writeChoice(char *s, int isSelected, int line);
void updateChoice(char **data, int o, int n, int old = -1, int nnew = -1);
int Load(Array &data);
int Save(Array data);
void resetFile();
int saveHigh(Array data);
int loadHigh(Array &data);
int resetHigh();