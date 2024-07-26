#include <iostream>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>

#define MAXSNAKESIZE 100
#define MAXFRAMEX 119
#define MAXFRAMEY 29

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
	
	void setcursor(bool visible, DWORD size) // establecer bool visible = 0 - invisible, bool visible = 1 - visible
	{
		if(size == 0)
		{
			size = 20;	// tama�o de cursor predeterminado Cambiar a n�meros de 1 a 20, disminuye el ancho del cursor
		}
		CONSOLE_CURSOR_INFO lpCursor;	
		lpCursor.bVisible = visible;
		lpCursor.dwSize = size;
		SetConsoleCursorInfo(console, &lpCursor);
	}
	
	class Point {
	private:
		int x;
		int y;
	public:
		Point() {
			x = y = 10;
		} 
		Point(int x, int y) {
			this->x = x;
			this->y = y;
		}
		void SetPoint(int x, int y) {
			this->x = x;
			this->y = y;
		}
		int GetX() {
			return x;
		}
		int GetY() {
			return y;
		}
		void MoveUp() {
			y--;
			if (y < 0)
				y = MAXFRAMEY;
		}
		void MoveDown() {
			y++;
			if (y > MAXFRAMEY)
				y = 0;
		}
		void MoveLeft() {
			x--;
			if (x < 0)
				x = MAXFRAMEX;
		}
		void MoveRight() {
			x++;
			if (x > MAXFRAMEX)
				x = 0;
		}
		void Draw(char ch = 'O') {
			gotoxy(x, y);
			cout << ch;
		}
		void Erase() {
			gotoxy(x, y);
			cout << " ";
		}
		void CopyPos(Point* p) {
			p->x = x;
			p->y = y;
		}
		int IsEqual(Point* p) {
			if (p->x == x && p->y == y)
				return 1;
			return 0;
		}
		void Debug() {
			cout << "(" << x << "," << y << ") ";
		}
	};
	
	class Snake {
	private:
		Point* cell[MAXSNAKESIZE]; // array de puntos para representar la serpiente
		int size; // tama�o actual de la serpiente
		char dir; // direcci�n actual de la serpiente
		Point fruit;
		int state; // bool que representa el estado de la serpiente, es decir, viva o muerta
		int started;
		int blink; // parpadeo de la fruta
	public:
		Snake() {
			size = 1; // tama�o predeterminado
			cell[0] = new Point(20, 20); // 20,20 es la posici�n predeterminada
			for (int i = 1; i < MAXSNAKESIZE; i++) {
				cell[i] = NULL;
			}
			fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
			state = 0;
			started = 0;
		}
		void AddCell(int x, int y) {
			cell[size++] = new Point(x, y);
		}
		void TurnUp() {
			if (dir != 's')
				dir = 'w'; // w es la tecla de control para girar hacia arriba
		}
		void TurnDown() {
			if (dir != 'w')
				dir = 's'; // w es la tecla de control para girar hacia abajo
		}
		void TurnLeft() {
			if (dir != 'd')
				dir = 'a'; // w es la tecla de control para girar hacia la izquierda
		}
		void TurnRight() {
			if (dir != 'a')
				dir = 'd'; // w es la tecla de control para girar hacia la derecha
		}
		void WelcomeScreen() {
			SetConsoleTextAttribute(console, 15);
			cout << "\n            /^\\/^\\                                             ";
			cout << "\n          _|__|  O|                                              ";
			cout << "\n \\/     /~     \\_/ \\                                          ";
			cout << "\n  \\____|__________/  \\                                         ";
			cout << "\n         \\_______      \\                                       ";
			cout << "\n                 `\\     \\                 \\                   ";
			cout << "\n                   |     |                  \\                   ";
			cout << "\n                  /      /                    \\                 ";
			cout << "\n                 /     /                       \\\\              ";
			cout << "\n               /      /                         \\ \\            ";
			cout << "\n              /     /                            \\  \\          ";
			cout << "\n            /     /             _----_            \\   \\        ";
			cout << "\n           /     /           _-~      ~-_         |   |          ";
			cout << "\n          (      (        _-~    _--_    ~-_     _/   |          ";
			cout << "\n           \\      ~-____-~    _-~    ~-_    ~-_-~    /     (o)  ";
			cout << "\n             ~-_           _-~          ~-_       _-~   - Nacho -";
			cout << "\n                ~--______-~                ~-___-~       - Dev - ";
		}
		void Move() {
			// Limpiar pantalla
			system("cls");
			
			if (state == 0) {
				if (!started) {
					WelcomeScreen();
					cout << "\n\nPresione cualquier tecla para comenzar :)";
					getch();
					started = 1;
					state = 1;
				}
				else {
					cout << "\nFin del Juego";
					cout << "\nPresione cualquier tecla para volver a comenzar :)";
					getch();
					state = 1;
					size = 1;
				}
			}
			
			// haciendo que el cuerpo de la serpiente siga a su cabeza
			for (int i = size - 1; i > 0; i--) {
				cell[i - 1]->CopyPos(cell[i]);
			}
			
			// girando la cabeza de la serpiente
			switch (dir) {
			case 'w':
				cell[0]->MoveUp();
				break;
			case 's':
				cell[0]->MoveDown();
				break;
			case 'a':
				cell[0]->MoveLeft();
				break;
			case 'd':
				cell[0]->MoveRight();
				break;
			}
			
			if (SelfCollision())
				state = 0;
			
			// Colisi�n con la fruta
			if (fruit.GetX() == cell[0]->GetX() && fruit.GetY() == cell[0]->GetY()) {
				AddCell(0, 0);
				fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
			}
			
			// dibujando la serpiente
			for (int i = 0; i < size; i++)
				cell[i]->Draw();
			
			SetConsoleTextAttribute(console, 242);
			if (!blink)
				fruit.Draw('#');
			blink = !blink;
			SetConsoleTextAttribute(console, 252);
			
			//Debug();
			
			Sleep(100);
		}
		int SelfCollision() {
			for (int i = 1; i < size; i++)
				if (cell[0]->IsEqual(cell[i]))
					return 1;
			return 0;
		}
		void Debug() {
			for (int i = 0; i < size; i++) {
				cell[i]->Debug();
			}
		}
	};
	
	int main() {
		// generaci�n de n�mero aleatorio
		setcursor(0, 0);
		srand((unsigned)time(NULL));
		
		// probando la serpiente
		Snake snake;
		char op = 'l';
		do {
			if (kbhit()) {
				op = getch();
			}
			switch (op) {
			case 'w':
			case 'W':
				snake.TurnUp();
				break;
				
			case 's':
			case 'S':
				snake.TurnDown();
				break;
				
			case 'a':
			case 'A':
				snake.TurnLeft();
				break;
				
			case 'd':
			case 'D':
				snake.TurnRight();
				break;
			}
			snake.Move();
		} while (op != 'e');
		
		return 0;
	}
	
	// EL FIN
	
