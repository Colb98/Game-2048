#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <thread>
#include "int.h"
#include "Console.h"
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "WinMM.lib")

using namespace std;


TCHAR tch[100];
int playingMusic(bool*, bool*);


int main() {
	Array data;
	HWND cs = GetConsoleWindow();
	bool play, eend;
	play = true;
	eend = false;
	thread t(playingMusic, &play, &eend);
	HANDLE handle_t = t.native_handle();
	ShowConsoleCursor(false);
	menu(data, &play);
	eend = true;
	t.join();
	delete[] data.data;
	return 0;
}

int playingMusic(bool *play, bool *eend) {
	bool pause = false;
	mciSendString("open theme.mid type sequencer alias Music1", NULL, 0, NULL);
	mciSendString("play Music1", NULL, 0, NULL);
	while (*eend == false) {
		while (*play && pause == false) {
			mciSendString("status Music1 mode", tch, 100, NULL);
			if (tch[0] == 's' && tch[1] == 't') {
				mciSendString("seek Music1 to start", NULL, 100, NULL);
				mciSendString("play Music1", NULL, 0, NULL);
			}
			if (*eend == true)
				return 0;
		}
		mciSendString("stop Music1", NULL, 0, NULL);
		pause = true;
		if (*play) {
			mciSendString("open theme.mid type sequencer alias Music1", NULL, 0, NULL);
			mciSendString("play Music1", NULL, 0, NULL);
			pause = false;
		}
	}
	return 0;
}
