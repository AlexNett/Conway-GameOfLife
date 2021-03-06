/*
Visitare alexnett.github.io/PAGES/GOL/GOL.html per documentazione
  ▀ ▀
 ▀▄▄▄▀
*/

#include <iostream>
#include <windows.h>
#include <process.h>
#include <string>
#include <conio.h>
using namespace std;
#define DARKGRAY SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
#define LIGHTRED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
#define YELLOW SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#define TL char(218) // TOP LEFT
#define TR char(191) // TOP RIGHT
#define BL char(192) // BOTTOM LEFT
#define BR char(217) // BOTTOM RIGHT
#define HL char(196) // HORIZONTAL LINE
#define VL char(179) // VERTICAL LINE
#define CB char(254) // BIG CUBE | ALIVE CELL 
#define NN char(250) // SMALL CUBE | DEAD CELL

int x = -1; // cordinate
int y = -1; // cordinate
int ROWS = 15; // quantità di righe nel canvas
int COLS = 15; // quantità di colonne nel canvas
char** cWorld; // canvas

// Start screen
void CMD_re_start(bool first)
{
	system("cls");
	YELLOW cout << "CMD: \n";
	if (first) { WHITE cout << "Type ["; LIGHTRED cout << "?"; WHITE cout << "] for command list. \n\n"; }
}

// Prima dell'input stampare un '>'
void CMD_input(char &c)
{
	YELLOW cout << ">";
	WHITE cin >> c;
}

// lista comandi
void help()
{
	LIGHTRED cout << "\n Net's Corway's game of life - v4.min\n\n";
	YELLOW
		cout << "  " << char(223) << ' ' << ' ' << char(223) << "\n";
	cout << " " << char(223) << char(220) << char(220) << char(220) << char(220) << char(223) << "\n\n";
	WHITE
		cout << " ["; LIGHTRED cout << "?"; WHITE cout << "]     Prints aviable commands \n";
	cout << " ["; LIGHTRED cout << "z"; WHITE cout << "]     Set size of the canvas \n";
	cout << " ["; LIGHTRED cout << "d"; WHITE cout << "]     Draw in the canvas \n";
	cout << " ["; LIGHTRED cout << "s"; WHITE cout << "]     Start simulation of the canvas \n";
	cout << " ["; LIGHTRED cout << "q"; WHITE cout << "]     Exits \n\n";
}

// stampa in modo leggibile il canvas
void print_canvas()
{
	YELLOW cout << TL;
	for (int i = 0; i < COLS; i++) { cout << HL << HL; }
	cout << HL << TR << endl;
	for (int i = 0; i < ROWS; i++) {
		cout << VL << ' ';
		for (int j = 0; j < COLS; j++) {
			WHITE
				if (j == x && i == y) { LIGHTRED }
			if (cWorld[i][j] == true) { cout << CB; }
			else {
				if (j == x && i == y) { cout << char(4); }
				else { DARKGRAY cout << NN; }
			}
			cout << ' ';
		}
		YELLOW cout << VL << endl;
	}
	cout << BL;
	for (int i = 0; i < COLS; i++) { cout << HL << HL; }
	cout << HL << BR << endl;
}

// modalità disegno
void draw()
{
	system("cls");
	x = 0; y = 0;
	while (true)
	{
		print_canvas();
		WHITE cout << " ["; LIGHTRED cout << "w-a-s-d"; WHITE cout << "]     Move cursor \n";
		WHITE cout << " ["; LIGHTRED cout << "SPACE"; WHITE cout << "]       Set life \n";
		WHITE cout << " ["; LIGHTRED cout << "ESC"; WHITE cout << "]         quit drawing \n";
		switch (_getch())
		{
		case 'a': x--; break;
		case 'd': x++; break;
		case 'w': y--; break;
		case 's': y++; break;
		case ' ': cWorld[y][x] = !cWorld[y][x]; break;
		case 27:
			x = -1; y = -1;
			CMD_re_start(false);
			return;
		}
		if (x < 0) { x = 0; }
		if (y < 0) { y = 0; }
		if (x > COLS - 1) { x = COLS - 1; }
		if (y > ROWS - 1) { y = ROWS - 1; }
		system("cls");
	}
}

// liberare memoria occupata dalla matrice
void free()
{
	for (int i = 0; i < ROWS; ++i) { delete[] cWorld[i]; }
	delete[] cWorld;
}

// allocare memoria con la matrice
void allocate()
{
	cWorld = new char*[ROWS];
	for (int i = 0; i < ROWS; i++) { cWorld[i] = new char[COLS]; }
	for (int i = 0; i < ROWS; i++) { for (int j = 0; j < COLS; j++) { cWorld[i][j] = 0; } }
}

// cambiare size del canvas
void set_size()
{
	free();
	LIGHTRED cout << '\t' << CB << " COLS: "; WHITE cin >> COLS;
	LIGHTRED cout << '\t' << CB << " ROWS: "; WHITE cin >> ROWS;
	allocate();
}

// funzione che ritorna quante celle vive sono adiacenti a quella nelle cordinate passate per parametro
int alive_near_ammount(int i, int j)
{
	int iConta = 0;
	for (int a = -1; a < 2; a++) {
		for (int b = -1; b < 2; b++) {
			if (a == 0 && b == 0) {}
			else if (
				cWorld[(i + ROWS + a) % ROWS][(j + COLS + b) % COLS] == 1 ||
				cWorld[(i + ROWS + a) % ROWS][(j + COLS + b) % COLS] == 2
				) {
				iConta++;
			}
		}
	}
	return iConta;
}

//  azioni per raggiungere una prossima generazione
void logic()
{
	int nAdiacenti = 0;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			nAdiacenti = alive_near_ammount(i, j);
			if ((nAdiacenti < 2 || nAdiacenti > 3) && cWorld[i][j] == 1) { cWorld[i][j] = 2; }
			else if ((nAdiacenti == 3) && cWorld[i][j] == 0) { cWorld[i][j] = 3; }
		}
	}
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (cWorld[i][j] == 2) { cWorld[i][j] = 0; }
			else if (cWorld[i][j] == 3) { cWorld[i][j] = 1; }
		}
	}

}

// stampa continua del canvas e effetuamento della logica cosi da vedere ogni passo delle generazioni
void work(void* n)
{
	int generation = 0;
	bool &stop = *((bool*)n);
	while (true)
	{
		print_canvas();
		WHITE cout << " ["; LIGHTRED cout << "s"; WHITE cout << "]       Stop/Start simulation \n";
		WHITE cout << " ["; LIGHTRED cout << "z"; WHITE cout << "]       1 Step in simulation \n";
		WHITE cout << " ["; LIGHTRED cout << "ESC"; WHITE cout << "]     quit simulation \n\n";
		WHITE cout << " Generation: ["; LIGHTRED cout << generation; WHITE cout << "]";
		while (stop != false) { if (stop == 'q') { return; } }
		Sleep(50);
		logic();
		generation++;
		system("cls");
	}
}

// modalità simulazione
void canvas()
{
	system("cls");
	char stop = true;
	HANDLE C = (HANDLE)_beginthread(&work, 0, &stop);
	while (true)
	{
		switch (_getch())
		{
		case 's': stop = !stop; break;
		case 'z': stop = false; Sleep(1); stop = true; break;
		case 27:
			stop = 'q';
			WaitForSingleObject(C, INFINITE);
			CMD_re_start(false);
			return;
		}
	}
}

// interfaccia dei comandi
int main()
{
	char cOption;
	allocate();
	CMD_re_start(true);
	while (true)
	{
		CMD_input(cOption);
		switch (cOption)
		{
		case '?': help(); break;
		case 'd': draw(); break;
		case 'z': set_size(); break;
		case 's': canvas(); break;
		case 'q': free(); return 0;
		}
	}
}