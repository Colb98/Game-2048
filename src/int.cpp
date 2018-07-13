#include <iostream>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <thread>
#include "Console.h"
#include "int.h"

using namespace std;

struct Stack {
	int *data;
	int cur;
	int max;
};

Console console;

bool validmove(Array a, int dir); //dir 0 1 2 3 up down left right
int getEnter(int *ans, bool *handle);
void Credit();
int Option(bool*, int, bool);
void printPlainMenu(Array a, char **text);
void tmpInstruction();
void clrIns();
void logo();
int PauseGame(Array &data, bool *music);

void Scored(Array &a, int val) {
	a.score += val;
	if (a.score > a.highscore)
		a.highscore = a.score;
	console.setColor(15);
	GotoXY(STARTX + DISTANCEX + 7, STARTY - DISTANCEY / 2 - 2);
	cout << a.score;
	console.setColor(11);
	GotoXY(STARTX + DISTANCEX + 25, STARTY - DISTANCEY / 2 - 2);
	cout << a.highscore;
}

void drawBoard(Array arr, int n) {
	console.setColor(15);
	system("CLS");	
	char a, b, c, d, e, f;
	a = 205;
	b = 186;
	c = 201;
	d = 187;
	e = 200;
	f = 188;
	
	GotoXY(STARTX + DISTANCEX, STARTY - DISTANCEY / 2 - 2);
	cout << "Score: " << arr.score;
	console.setColor(11);
	GotoXY(STARTX + DISTANCEX + 13, STARTY - DISTANCEY / 2 - 2);
	cout << "High Score: ";
	if (arr.highscore)
		cout << arr.highscore;
	console.setColor(14);
	GotoXY(STARTX - DISTANCEX/2 - 2, STARTY - DISTANCEY/2 - 1);
	cout << c << a;
	for (int i = 0;i < n;i++)
		for (int j = 0;j < DISTANCEX;j++)
			cout << a;
	cout << a << d;
	GotoXY(STARTX - DISTANCEX / 2 - 2, STARTY + n*DISTANCEY - DISTANCEY/2);
	cout << e << a;
	for (int i = 0;i < n;i++)
		for (int j = 0;j < DISTANCEX;j++)
			cout << a;
	cout << a << f;
	for(int i=0;i<n;i++)
		for (int j = 0;j < DISTANCEY;j++) {
			GotoXY(STARTX - DISTANCEX / 2 - 2, STARTY - DISTANCEY / 2 + i*DISTANCEY + j);
			cout << b;
		}
	for (int i = 0;i<n;i++)
		for (int j = 0;j < DISTANCEY;j++) {
			GotoXY(STARTX - DISTANCEX / 2 + n*DISTANCEX + 1, STARTY - DISTANCEY / 2 + i*DISTANCEY + j);
			cout << b;
		}
}

Stack* initStack(Array a, int n, int dir) { //dir = 0 1 2 3 <-> up down left right
	Stack *des = new Stack;
	int cur = 0;
	des->data = new int[a.n];
	for (int i = 0;i < a.n;i++)
		des->data[i] = 0;
	if (dir == 1)
		for (int i = 0;i < a.n;i++) { //Tren xuong, doc tu dong 0->n-1
			if (a[i][n])
				des->data[cur++] = a[i][n];
		}
	else if (dir == 0)
		for (int i = 0;i < a.n;i++) {
			if (a[a.n - i - 1][n])
				des->data[cur++] = a[a.n - i - 1][n];
		}
	else if (dir == 2)
		for (int i = 0;i < a.n;i++){
			if(a[n][i])
				des->data[cur++] = a[n][i];
		}
	else
		for (int i = 0;i < a.n;i++) {
			if (a[n][a.n - i - 1])
				des->data[cur++] = a[n][a.n - i - 1];
		}
	des->max = cur - 1; 
	des->cur = 0;
	return des;
}

void freeStack(Stack *des) {
	delete[] des->data;
	delete des;
}

int pop(Stack *a) {
	a->cur++;
	return a->data[a->cur - 1];
}

bool isEmpty(Stack *a) {
	return (a->cur > a->max);
}

int peek(Stack *a) {
	return a->data[a->cur];
}

void printStack(Stack *a) {
	GotoXY(0, 1);
	for (int i = 0;i < 4;i++)
		cout << a->data[i] << " ";
	cout << "cur = " << a->cur << endl;
}

void clear() {
	GotoXY(0, 1);
	cout << "                         ";
}

int countDigit(int a) {
	int ans = 0;
	if (a == 0) return 1;
	while (a > 0)
		a /= 10, ans++;
	return ans;
}

bool intInit(Array &a, int n) {
	if (a.data != NULL)
		delete[] a.data;
	a.n = n;
	a.score = 0;
	a.data = new int[n*n];
	if (a.data == NULL) 
		return false;
	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			a[i][j] = 0;
	return true;
}


//void printTable(Array &a) {
//	system("cls");
//	int n = a.n;
//	for (int i = 0;i < n;i++)
//		for (int j = 0;j < n;j++) {
//			// Count number to center align
//			int stepback = countDigit(a[i][j]) / 2;
//			GotoXY(STARTX + j*DISTANCEX - stepback, STARTY + (n-1-i)*DISTANCEY);
//			cout << a[i][j];
//		}
//	return;
//}

void printTable(Array &a) {
	for(int i=0;i<a.n;i++)
		for(int j=0;j<a.n;j++)
			drawTile(a[i][j], a.n, j, i, 0, 0);
}


void drawTile(int val, int n, int x, int y, int dx, int dy) { //x tuong ung voi cot -> j
	//dy = +1 thi dich xuong
	//dx = +1 thi dich qua phai

	int stepback = countDigit(val) / 2; //step back to center number
	
	unsigned short multipler = log2(val);
	int topleftx = STARTX + x*DISTANCEX - DISTANCEX / 2;
	int toplefty = STARTY + (n - 1 - y)*DISTANCEY - DISTANCEY / 2;
	dx *= (DISTANCEX / 2);
	dy *= (DISTANCEY / 2);
	topleftx += dx;
	toplefty += dy;	
	
	if (multipler == 1)
		console.setColor(241);
	else
		console.setColor((multipler * 15) % 256);
	for (int i = 0;i < DISTANCEY;i++) {
		GotoXY(topleftx, toplefty + i);
		for (int j = 0;j < DISTANCEX;j++)
			cout << ' ';		
	}
	GotoXY(STARTX + x*DISTANCEX - stepback + dx, STARTY + (n - 1 - y)*DISTANCEY + dy);
	if(val) cout << val;
}

int gameFunc(Array & data, bool &gen)
{
	bool flag = false;
	while (!isFull(data) && !flag) {
		if (gen)
			genElement(data);
		else
			gen = true;
		if (isFull(data))
			return 1;
		printTable(data);
		char c; bool validmove = false;
		do {
			do {
				c = _getch();
				c = toupper(c);
			} while (c != 'A' && c != 'W' && c != 'S' && c != 'D' && c != 27);
			switch (c) {
			case 'W': validmove = moveUp(data); break;
			case 'S': validmove = moveDown(data); break;
			case 'A': validmove = moveLeft(data); break;
			case 'D': validmove = moveRight(data); break;
			case 27: flag = true; break;
			}
		} while (!validmove && !flag && !isFull(data));
		if (isFull(data))
			return 1;
	}
	if(flag) return 0; //pause game
	return 1; //game over
}

void genElement(Array &a) {
	srand((int)time(NULL));
	int n = a.n, i;
	int ran1 = rand() % 100;	
	int num, countblank = 0;
	//Decide new number is 2 or 4
	if (ran1 < RATIO)
		num = 2;
	else
		num = 4;
	//Decide where to put new number
	for (i = 0;i < n*n;i++)
		if (a.data[i] == 0) countblank++;
	if (countblank == 0) return;
	int ran2 = rand() % countblank; //ran2 would be in range 0..countblank-1 
	i = -1;
	while (ran2 >= 0) {
		i++;
		if (a.data[i] == 0) ran2--;
	}
	a.data[i] = num;
	return;
}

bool isFull(Array a) {
	for (int i = 0;i < a.n*a.n;i++)
		if (a.data[i] == 0)
			return false;
	for (int i = 0;i < 4;i++)
		if (validmove(a, i))
			return false;
	return true;
}

void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ShowConsoleCursor(bool showFlag) // source: StackoverFlow
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

bool moveDown(Array &a) {
	bool valid = false;
	int n = a.n;
	if (validmove(a, 1)) valid = true;
	for (int i = 0;i < n;i++) {
		//Column i-th
		int j = 0; //Cursor to current row to fill number in
		Stack *tmp = initStack(a, i, 1); 
		while (!isEmpty(tmp)) {			
			a[j][i] = pop(tmp);
			if (a[j][i] == peek(tmp)) {
				/*int k = j + 1;
				while (a[k][i] != peek(tmp))
					k++;
				float di = (float)(k - j)/ 2;
				for (;k < n;k++)
					if (a[k][i]) {
						drawTile(0, n, i, k, 0, 0);
						drawTile(a[k][i], n, i, k, 0, di);
					}
				Sleep(DELAYMOVING);*/
				Scored(a, a[j][i]);
				a[j][i] *= 2, pop(tmp);
				valid = true;
			}
			j++; 
		}
		while (j < n)
			a[j++][i] = 0;
		freeStack(tmp);
	}
	return valid;
}

bool moveUp(Array &a) {
	bool valid = false;
	int n = a.n;
	if (validmove(a, 0)) valid = true;
	for (int i = 0;i < n;i++) {
		//Column i-th
		int j = n-1; //Cursor to current row to fill number in
		Stack *tmp = initStack(a, i, 0);
		while (!isEmpty(tmp)) {
			a[j][i] = pop(tmp);
			if (a[j][i] == peek(tmp)) {
				/*int k = j - 1;
				while (a[k][i] != peek(tmp))
					k--;
				float di = (float)(k - j)/ 2;
				for (;k >= 0;k--)
					if (a[k][i]) {
						drawTile(0, n, i, k, 0, 0);
						drawTile(a[k][i], n, i, k, 0, di);
					}
				Sleep(DELAYMOVING);*/
				Scored(a, a[j][i]);
				a[j][i] *= 2, pop(tmp);
				valid = true;
			}
			j--;
		}
		while (j >= 0)
			a[j--][i] = 0;
		freeStack(tmp);
	}
	return valid;
}

bool moveLeft(Array &a) {
	bool valid = false;
	int n = a.n;
	if (validmove(a, 2)) valid = true;
	else return false;
	for (int i = 0;i < n;i++) {
		//Row i-th
		int j = 0; //Cursor to current column to fill number in
		Stack *tmp = initStack(a, i, 2);
		while (!isEmpty(tmp)) {
			a[i][j] = pop(tmp);
			if (a[i][j] == peek(tmp)) {
				/*int k = j + 1;
				while (a[i][k] != peek(tmp))
					k++;
				float di = (float)(k - j) / 2;
				for (;k < n;k++)
					if (a[i][k]) {
						drawTile(0, n, k, i, 0, 0);
						drawTile(a[i][k], n, k, i, 0, di);
					}
				Sleep(DELAYMOVING);*/
				Scored(a, a[j][i]);
				a[i][j] *= 2, pop(tmp);
				valid = true;
			}
			j++;
		}
		while (j < n)
			a[i][j++] = 0;
		freeStack(tmp);
	}
	return valid;
}

bool moveRight(Array &a) {
	bool valid = false;
	int n = a.n;
	if (validmove(a, 3)) valid = true;
	for (int i = 0;i < n;i++) {
		//Row i-th
		int j = n - 1; //Cursor to current column to fill number in
		Stack *tmp = initStack(a, i, 3);
		while (!isEmpty(tmp)) {
			a[i][j] = pop(tmp);
			if (a[i][j] == peek(tmp)) {
				/*int k = j - 1;
				while (a[i][k] != peek(tmp))
					k--;
				float di = (float)(k - j) / 2;
				for (;k >= 0;k--)
					if (a[i][k]) {
						drawTile(0, n, k, i, 0, 0);
						drawTile(a[i][k], n, k, i, 0, di);
					}
				Sleep(DELAYMOVING);*/
				Scored(a, a[j][i]);
				
				a[i][j] *= 2, pop(tmp);
				valid = true;
			}
			j--;
		}
		while (j >= 0)
			a[i][j--] = 0;
		freeStack(tmp);
	}
	return valid;
}

bool validmove(Array a, int dir) {
	int startR, startC, endR, endC;
	int n = a.n, di;
	switch (dir) {
	case 0: startR = 0; endR = n - 1; startC = 0; endC = n - 1; di = 1; break;
	case 1: startR = n - 1; endR = 0; startC = 0; endC = n - 1; di = -1; break;
	case 2: startR = 0; endR = n - 1; startC = n - 1; endC = 0; di = -1; break;
	case 3: startR = 0; endR = n - 1; startC = 0; endC = n - 1; di = 1; break;
	}
	if (dir <= 1) { //xet tung cot khi tren xuong
		for (int i = startC;i <= endC;i++) 
			for (int j = startR;j != endR;j += di)
				if ((a[j][i] != 0 && a[j + di][i] == 0) ||
					(a[j][i] == a[j+di][i] && a[j][i] && a[j+di][i])) return true;
	}
	else //xet tung dong khi trai phai
		for (int i = startR;i <= endR;i++)
			for (int j = startC;j != endC;j += di)
				if ((a[i][j] != 0 && a[i][j + di] == 0) ||
					(a[i][j] == a[i][j+di] && a[i][j] && a[i][j+di])) return true;
	return false;
}


// Plays a specified MIDI file by using MCI_OPEN and MCI_PLAY. Returns 
// as soon as playback begins. The window procedure function for the 
// specified window will be notified when playback is complete. 
// Returns 0L on success; otherwise, it returns an MCI error code.

DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName)
{
	UINT wDeviceID;
	DWORD dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
	MCI_SEQ_SET_PARMS mciSeqSetParms;

	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.
	mciOpenParms.lpstrDeviceType = "sequencer";
	mciOpenParms.lpstrElementName = lpszMIDIFileName;
	if (dwReturn = mciSendCommand(NULL, MCI_OPEN,
		MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD)(LPVOID)&mciOpenParms))
	{
		// Failed to open device. Don't close it; just return error.
		return (dwReturn);
	}

	// The device opened successfully; get the device ID.
	wDeviceID = mciOpenParms.wDeviceID;

	// Check if the output port is the MIDI mapper.
	mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	// The output port is not the MIDI mapper. 
	// Ask if the user wants to continue.
	//if (LOWORD(mciStatusParms.dwReturn) != MIDI_MAPPER)
	//{
	//	if (MessageBox(hWndNotify,
	//		"The MIDI mapper is not available. Continue?",
	//		"", MB_YESNO) == IDNO)
	//	{
	//		// User does not want to continue. Not an error;
	//		// just close the device and return.
	//		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
	//		return (0L);
	//	}
	//}

	// Begin playback. The window procedure function for the parent 
	// window will be notified with an MM_MCINOTIFY message when 
	// playback is complete. At this time, the window procedure closes 
	// the device.
	mciPlayParms.dwCallback = (DWORD)hWndNotify;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
		(DWORD)(LPVOID)&mciPlayParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	return (0L);
}

int menu(Array &data, bool *music) {
	HWND cs = GetConsoleWindow();
	static bool gen = true;
	int n = DEFAULT_SQUARE;
	int choice = 1; //(Resume,) Start game, High Score, Credit, Option, Exit
	int ans = 0;
	//static int firstPress = 0;
	char src[][20] = { "Resume", "New Game", "Reset High Score", "Credit", "Option", "Exit" };
	char **text = new char*[6];
	for (int i = 0;i < 6;i++) {
		text[i] = new char[strlen(src[i])+1];
		strcpy(text[i], src[i]);
	}
	int isSaved;
	isSaved = Load(data);
	//tmpInstruction();
	//Sleep(1000);
	loadHigh(data);
	char c;
	while (choice != 5) {
		
		if (n != data.n){ // If changed size in data
			data.score = 0;
		}
		if (isSaved || data.score) 
			choice = 0;
		printPlainMenu(data, text);
		while ((c = _getch()) != 13) {

			c = toupper(c);
			if (c == 'W' || c == 'A') {
				//firstPress = 1;
				int tmp = choice;
				choice--;
				if (isSaved || data.score) {
					if (choice < 0) choice = 5;
				}
				else
					if (choice < 1) choice = 5;

				updateChoice(text, tmp, choice, tmp+2, choice+2);
				//Sleep(200);
			}
			if (c == 'S' || c == 'D') {
				//firstPress = 1;
				int tmp = choice;
				choice++;
				if (choice == 6)
					if (isSaved || data.score)
						choice = 0;
					else
						choice = 1;
				updateChoice(text, tmp, choice, tmp+2, choice+2);
				//Sleep(200);
			}
			//if (firstPress == 1)
				//clrIns(), firstPress = 2;
		}
		if (choice == 1 || choice == 0) {
			int pause = 1; 
			if (choice == 1) {
				intInit(data, n);
				resetFile();
				isSaved = 0;
				gen = true;
			}
			else
				gen = false;
			while (pause == 1){ //keep repeat playing while in game/pause menu
				//if pause==2 then back to main menu				
				
				drawBoard(data, n);
				printTable(data);
				int game = gameFunc(data, gen); //Gamefunc return 0 means just pause, 1 means over
				//Pause here
				if (game == 1) {
					data.score = 0;
					pause = 2;
				}
				else do {
					pause = PauseGame(data, music);
					if (pause == 3)
						Option(music, n, false);
				} while (pause == 3);
				gen = false;
			}
			//printPlainMenu(data, text);
		}
		if (choice == 4) {
			int tmp = n;
			n = Option(music, tmp, true);
			if (tmp != n) {
				resetFile();
				isSaved = 0;
				data.score = data.highscore = 0;
			}
			if (isSaved || data.score)
				choice = 0;
			else 
				choice = 1;
			//printPlainMenu(data, text);
		}
		if (choice == 3) {
			Credit();
			if (isSaved || data.score)
				choice = 0;
			else
				choice = 1;
			//printPlainMenu(data, text);
		}			
		if (choice == 2) {
			resetHigh();
			resetFile();
			isSaved = 0;
			data.score = data.highscore = 0;
			choice = 1;
		}
	}
	saveHigh(data);
	for (int i = 0;i < 6;i++)
		delete[] text[i];
	delete[] text;
	return 0;
}

void updateChoice(char **data, int o, int n, int old, int nnew) {
	if (old == -1)
		old = o;
	if (nnew == -1)
		nnew = n;
	writeChoice(data[o], 0, old);
	writeChoice(data[n], 1, nnew);
}


void resetFile() {
	FILE *f = fopen(DATAFILE, "wb");
	fclose(f);
	return;
}

int Load(Array &data) // return 0 if there is no save, return 1 if there is a game
{	
	FILE *f;
	f = fopen(DATAFILE, "rb");
	if (f == NULL) {
		resetFile();
		return 0;
	}	
	else {
		if (fread(&data.n, sizeof(data.n), 1, f)) {
			fread(&data.score, sizeof(data.score), 1, f);
			int n = data.n;
			data.data = new int[n*n];
			fread(data.data, sizeof(int), n*n, f);
			fclose(f);
			return 1;
		}
		else return 0;
	}
}


int Save(Array data) {
	FILE* f = fopen(DATAFILE, "wb");
	if (f == NULL) return 0;
	//fwrite(&data.highscore, sizeof(int), 1, f);
	fwrite(&data.n, sizeof(int), 1, f);
	fwrite(&data.score, sizeof(int), 1, f);
	fwrite(data.data, sizeof(int), data.n*data.n, f);
	fclose(f);
	return 1;
}

int saveHigh(Array data) {
	int high = data.highscore;
	FILE *f = fopen(HSFILE, "wb");
	if (f == NULL) return 0;
	fwrite(&high, sizeof(int), 1, f);
	fclose(f);
	return 1;
}

int resetHigh() {
	FILE *f = fopen(HSFILE, "wb");
	int high = 0;
	if (f == NULL) return 0;
	fwrite(&high, sizeof(int), 1, f);
	fclose(f);
	return 1;
}

int loadHigh(Array &data) {
	FILE *f = fopen(HSFILE, "rb");
	if (f == NULL) return 0;
	fread(&data.highscore, sizeof(int), 1, f);
	fclose(f);
	return 1;
}

void writeChoice(char *s, int isSelected, int line) {
	int len = strlen(s);
	GotoXY(50 - len / 2, 5 + line * 3);
	if (isSelected == 3) {
		int r = rand() % 7 + 9;
		console.setColor(r);
	}
	else if (isSelected == 2)
		console.setColor(249);
	else if (isSelected == 1)
		console.setColor(233);
	else
		console.setColor(15);
	cout << s;
}

int getEnter(int *ans, bool *handle) {
	char c = 0;
	while (handle) {
		c = _getch();
		if (c == 13) *ans = 1;
	}
	return 0;
}

void Credit() {
	console.setColor(15);
	system("CLS");
	writeChoice("Credit", 2, 0);
	writeChoice("2048  Game", 1, 2);
	writeChoice("Creator: Nguyen Buu Loc, HCMUS", 3, 3);
	writeChoice("Based on idea of PMS", 3, 4);
	writeChoice("Have fun!!", 3, 5);
	writeChoice("Version: 1.0.0", 3, 6);
	writeChoice("Press any key to return to main menu", 3, 7);
	_getch();
}

void printPlainMenu(Array a, char **text) {
	console.setColor(15);
	system("CLS");
	logo();
	console.setColor(15);
	if (a.score != 0) {
		writeChoice(text[0], 1, 2);
		writeChoice(text[1], 0, 3);
	}
	else
		writeChoice(text[1], 1, 3);
	writeChoice(text[2], 0, 4);
	writeChoice(text[3], 0, 5);
	writeChoice(text[4], 0, 6);
	writeChoice(text[5], 0, 7);
	console.setColor(14);
	GotoXY(59, 17);
	if (a.highscore != 0) cout << a.highscore;
}

void tmpInstruction() {
	console.setColor(15);
	GotoXY(70, 14);
	cout << "Use A S D to navigate in menu and play";
	console.setColor(11);
	GotoXY(76, 13);
	cout << "W";
	GotoXY(74, 14);
	cout << "A S D";
}

void clrIns() {
	console.setColor(15);
	GotoXY(76, 13);
	cout << " ";
	GotoXY(70, 14);
	cout << "                                      ";
}

int Option(bool *music, int size, bool changeSize) {
	int ans = 4;
	int choice = 0;
	char c = 0;
	console.setColor(15);
	char src[][30] = { "Music ON", "Square size", "Back to previous menu", "Music OFF" };
	char **text = new char*[4];
	for (int i = 0;i < 4;i++) {
		text[i] = new char[strlen(src[i])+1];
		strcpy(text[i], src[i]);
	}
	system("CLS");
	writeChoice("Option", 2, 0);
	if (*music)
		writeChoice("Music ON", 1, 2);
	else
		writeChoice("Music OFF", 1, 2);
	writeChoice("Square size", 0, 3);
	writeChoice("Back to previous menu", 0, 4);
	console.setColor(15);
	GotoXY(57, 14);
	cout << size;
	while (choice != 2) {
		while ((c = _getch()) != 13) {
			c = toupper(c);
			if (c == 'W' || c == 'A') {
				int tmp1 = choice, tmp2;
				int tmp = choice;
				if (tmp == 0)
					if (*music)
						tmp1 = 0;
					else
						tmp1 = 3;
				choice--; 
				if (choice < 0) choice = 2;
				tmp2 = choice;
				if (choice == 0)
					if (*music)
						tmp2 = 0;
					else
						tmp2 = 3;
				updateChoice(text, tmp1, tmp2, tmp + 2, choice + 2);
				//Sleep(200);
				continue;
			}
			if (c == 'S' || c == 'D') {
				int tmp1 = choice, tmp2;
				int tmp = choice;
				if (tmp == 0)
					if (*music)
						tmp1 = 0;
					else
						tmp1 = 3;
				choice++; 
				if (choice == 3) choice = 0;
				tmp2 = choice;
				if (choice == 0)
					if (*music)
						tmp2 = 0;
					else
						tmp2 = 3;
				updateChoice(text, tmp1, tmp2, tmp + 2, choice + 2);
				//Sleep(200);
				continue;
			}
		}
		if (choice == 0) {
			*music = !*music;
			console.setColor(80);
			GotoXY(52, 11);
			if (*music) {
				cout << "ON";
				console.setColor(15);
				cout << ' ';
			}
			else
				cout << "OFF";
		}
		if (choice == 1 && changeSize) {
			ShowConsoleCursor(true);
			console.setColor(15);
			GotoXY(57, 14);
			cout << ' ';
			cin >> ans;
			ShowConsoleCursor(false);
		}
	}
	
	for (int i = 0;i < 4;i++)
		delete[] text[i];
	delete[] text;
	return ans;
}

void logo() {
	//Number 2
	console.setColor(9);
	GotoXY(LOGOX + 1, LOGOY);
	cout << "22";
	GotoXY(LOGOX, LOGOY + 1);
	cout << "2  2";
	GotoXY(LOGOX + 2, LOGOY + 2);
	cout << "2";
	GotoXY(LOGOX + 1, LOGOY + 3);
	cout << "2";
	GotoXY(LOGOX, LOGOY + 4);
	cout << "2222";
	//Number 0
	console.setColor(14);
	GotoXY(LOGOX + 6, LOGOY + 1);
	cout << "000";
	GotoXY(LOGOX + 5, LOGOY + 2);
	cout << "0   0";
	GotoXY(LOGOX + 5, LOGOY + 3);
	cout << "0   0";
	GotoXY(LOGOX + 6, LOGOY + 4);
	cout << "000";
	//Number 4
	console.setColor(12);
	GotoXY(LOGOX + 15, LOGOY);
	cout << "4";
	GotoXY(LOGOX + 14, LOGOY + 1);
	cout << "4";
	GotoXY(LOGOX + 13, LOGOY + 2);
	cout << "4";
	GotoXY(LOGOX + 12, LOGOY + 3);
	cout << "4  4";
	GotoXY(LOGOX + 11, LOGOY + 4);
	cout << "44444";
	//Number 8
	console.setColor(11);
	GotoXY(LOGOX + 18, LOGOY);
	cout << "888";
	GotoXY(LOGOX + 17, LOGOY + 1);
	cout << "8   8";
	GotoXY(LOGOX + 18, LOGOY + 2);
	cout << "888";
	GotoXY(LOGOX + 17, LOGOY + 3);
	cout << "8   8";
	GotoXY(LOGOX + 18, LOGOY + 4);
	cout << "888";
}

int PauseGame(Array &data, bool *music) {
	int choice = 0;
	char c = 0;
	char src[][20] = { "Resume", "Option", "Save Game", "Back to Main Menu"};
	char **text = new char*[4];
	for (int i = 0;i < 4;i++) {
		text[i] = new char[strlen(src[i]) + 1];
		strcpy(text[i], src[i]);
	}
	console.setColor(15);
	system("CLS");
	while (1) {
		writeChoice("Game Paused", 2, 0);
		writeChoice(text[0], 1, 2);
		writeChoice(text[1], 0, 3);
		writeChoice(text[2], 0, 4);
		writeChoice(text[3], 0, 5);
		while ((c = _getch()) != 13) {
			c = toupper(c);
			if (c == 'W' || c == 'A') {
				int tmp = choice;
				choice--;
				if (choice < 0) choice = 3;
				updateChoice(text, tmp, choice, tmp+2, choice+2);
			}
			if (c == 'S' || c == 'D') {
				int tmp = choice;
				choice++;
				if (choice == 4) choice = 0;
				updateChoice(text, tmp, choice, tmp+2, choice+2);
			}
		}
		if (choice == 0)
			return 1; //Means continue
		if (choice == 3)
			return 2; //Means back to Main menu
		if (choice == 1)
			return 3; //Means go to Option
		if (choice == 2) {
			Save(data);
			return 1;
		}
	}
	return 0;
}