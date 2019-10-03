//02102019 minesweeper game
#include<iostream>
using namespace std;
#include<conio.h>
#include<iomanip>
#include<Windows.h>

/*
ввел структуру Ячейка (cell), теперь игра построена на понятной логике без кодирования каждой операции,
как было в предыдущей версии
улучшена функция открытия ячейки, повторяющиеся действия выведены в отдельную функцию
*/

struct cell
{
	bool isBomb;
	bool isOpen;
	bool isFlag;
	short content;
};

void createCell(cell*&);

void deleteCell(cell*&);

void createCellArr2D(cell***&, const int, const int);

void deleteCellArr2D(cell***&, const int, const int);

template<typename T>
T getRand(const T, const T, const int);

void setBombs(cell***, const int, const int, int);

void setHints(cell***, const int, const int);

void showGameField(const cell* const* const* const, const int, const int, const int, const int, const int, int, const int);

void makeUserMove(cell***, int&, int&, const int, const int, int&, int&, int&, int&, int&);

void openCell(cell***, const int, const int, const int, const int);

void checkNearbyCells(cell***, const int, const int, const int, const int, const int, const int, const int, const int);

void addScore(int&, int&);

bool winGame(cell const* const* const*, const int, const int);

int main()
{
	int row{};
	int col{};
	int qttOfBombs{ 10 };
	int qttOfFlags = qttOfBombs;
	cout << "Enter size of game field (from 5 to 10): ";
	cin >> row;
	col = row;

	int qttOfMoves{ col * row };
	int lifes{ 3 };
	int score{};
	int scoreCounter{-1};

	int x{ col / 2 }; //x and y are users coordinates for cursor
	int y{ row / 2 };

	cell*** gameField = new cell** [row] {};
	createCellArr2D(gameField, row, col);
	
	setBombs(gameField, row, col, qttOfBombs);

	setHints(gameField, row, col);
	
	while (true)
	{
		if (!qttOfMoves || !lifes || winGame(gameField, row, col)) break;
		else
		{
			showGameField(gameField, x, y, row, col, qttOfMoves, lifes, score);
			makeUserMove(gameField, x, y, row, col, qttOfMoves, lifes, score, scoreCounter, qttOfFlags);
		}
	}	
	showGameField(gameField, x, y, row, col, qttOfMoves, lifes, score);

	deleteCellArr2D(gameField, row, col);

	return 0;
}

void createCell(cell*& temp)
{
	if (!temp)
	{
		temp = new cell{};
		temp->isBomb = false;
		temp->isOpen = false;
		temp->isFlag = false;
		temp->content = 0;
	}
}

void deleteCell(cell*& temp)
{
	if (temp)
	{
		delete temp;
		temp = nullptr;
	}
}


void createCellArr2D(cell***& arr, const int row, const int col)
{
	while (true)
	{
		if (!arr)
		{
			arr = new cell** [row] {};
		}
		else break;
	}

	if (arr)
	{
		for (int i = 0; i < row; i++)
		{
			*(arr + i) = nullptr;
			while (*(arr + i) == nullptr)
			{
				*(arr + i) = new cell* [col] {};
				for (int j = 0; j < col; j++)
				{
					*(*(arr + i) + j) = nullptr;
					createCell(*(*(arr + i) + j));
				}
			}
		}
	}
}

void deleteCellArr2D(cell***& arr, const int row, const int col)
{
	if (arr)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{				
				deleteCell(*(*(arr + i) + j));
			}
			delete[] *(arr + i);
		}
		delete[] arr;
		arr = nullptr;
	}
}

template<typename T>
T getRand(const T min, const T max, const int order)
{
	return (T)((rand() % (int)(max - min) * order) + (int)min * order) / order;
}

void setBombs(cell*** arr, const int row, const int col, int bombs)
{
	int x{};
	int y{};
	while (bombs)
	{
		x = getRand(0, col, 1);
		y = getRand(0, row, 1);
		if ((*(*(arr + y) + x))->isBomb == false)
		{
			(*(*(arr + y) + x))->isBomb = true;
			bombs--;
		}
	}
}

void setHints(cell*** arr, const int row, const int col)
{
	int bombCounter{};
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (!(*(*(arr + i) + j))->isBomb) //if field is not bomb than check for bombs around
			{
				if (i == 0 && j == 0)
				{
					if ((*(*(arr + i) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j))->isBomb) bombCounter++;
				}
				else if (i == 0 && j == col - 1)
				{
					if ((*(*(arr + i) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j))->isBomb) bombCounter++;
				}
				else if (i == row - 1 && j == 0)
				{
					if ((*(*(arr + i - 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j + 1))->isBomb) bombCounter++;
				}
				else if (i == row - 1 && j == col - 1)
				{
					if ((*(*(arr + i - 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j - 1))->isBomb) bombCounter++;
				}
				else if (i == 0)
				{
					if ((*(*(arr + i) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j + 1))->isBomb) bombCounter++;
				}
				else if (i == row - 1)
				{
					if ((*(*(arr + i - 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j + 1))->isBomb) bombCounter++;
				}
				else if (j == 0)
				{
					if ((*(*(arr + i - 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j))->isBomb) bombCounter++;
				}
				else if (j == col - 1)
				{
					if ((*(*(arr + i - 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j))->isBomb) bombCounter++;
				}
				else
				{
					if ((*(*(arr + i - 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i - 1) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i) + j + 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j - 1))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j))->isBomb) bombCounter++;
					if ((*(*(arr + i + 1) + j + 1))->isBomb) bombCounter++;
				}

				(*(*(arr + i) + j))->content = bombCounter;  //set hint in cell content
				bombCounter = 0;
			}
		}
	}
}

void showGameField(const cell* const* const* const arr, const int x, const int y, const int row, const int col, const int qttOfMoves, int lifes, const int score)
{
	system("cls");
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i == y && j == x) cout << (char)206;
			else if ((*(*(arr + i) + j))->isFlag) cout << (char)16;
			else if (!(*(*(arr + i) + j))->isOpen) cout << (char)178;
			else if ((*(*(arr + i) + j))->isOpen && !(*(*(arr + i) + j))->isBomb && (*(*(arr + i) + j))->content >= 1 && (*(*(arr + i) + j))->content <= 8) cout << (*(*(arr + i) + j))->content;
			else if ((*(*(arr + i) + j))->isOpen && !(*(*(arr + i) + j))->isBomb && (*(*(arr + i) + j))->content == 0) cout << (char)0;
			else if ((*(*(arr + i) + j))->isOpen && (*(*(arr + i) + j))->isBomb) cout << (char)15;
		}
		cout << endl;
	}

	cout << "Moves left: " << qttOfMoves;
	cout << setw(20) << "Lifes left: ";
	while (lifes--)
	{
		cout << (char)3;
	}
	cout << "\nYour score: " << score << endl;
}

void makeUserMove(cell*** arr, int& x, int& y, const int row, const int col, int& qttOfMoves, int& lifes, int& score, int& scoreCounter, int& qttOfFlags)
{
	char userMove{};
	userMove = _getch();
	switch (userMove)
	{
	case 'w':
	{
		if (y == 0) y = row - 1;
		else y -= 1;
	}
	break;

	case 's':
	{
		if (y == row - 1) y = 0;
		else y += 1;
	}
	break;

	case 'a':
	{
		if (x == 0) x = col - 1;
		else x -= 1;
	}
	break;

	case 'd':
	{
		if (x == col - 1) x = 0;
		else x += 1;
	}
	break;

	case 'f': //for setting and unsetting flag
	{
		if ((*(*(arr + y) + x))->isFlag)
		{
			(*(*(arr + y) + x))->isFlag = false;
			qttOfMoves--;
			qttOfFlags++;
		}
		else if(qttOfFlags > 0)
		{
			(*(*(arr + y) + x))->isFlag = true;
			qttOfMoves--;
			qttOfFlags--;
		}
	}
	break;

	case ' ': //for opening a field
	{
		if (!(*(*(arr + y) + x))->isOpen && !(*(*(arr + y) + x))->isFlag)
		{
			if ((*(*(arr + y) + x))->isBomb)
			{
				lifes--;
				score -= 5;
			}
			else if (!(*(*(arr + y) + x))->isBomb)
			{
				addScore(score, scoreCounter);
			}

			openCell(arr, row, col, x, y);
			qttOfMoves--;
		}
	}
	break;

	default: break;
	}
}

void openCell(cell*** arr, const int row, const int col, const int x, const int y)
{
	if (!(*(*(arr + y) + x))->isFlag && !(*(*(arr + y) + x))->isOpen && !(*(*(arr + y) + x))->isBomb && (*(*(arr + y) + x))->content == 0)  //if try to open empty field without bomb and without hint
	{
		(*(*(arr + y) + x))->isOpen = true;

		int xOffset{};  //offsets are int from "-1" to "1" for checking cells on the right, on the left, below, above selected cell and diagonal cells
		int yOffset{};
		int yOffsetDiag{};
		int xOffsetDiag{};

		xOffset = 1;
		yOffset = 0;  //checking right cell
		xOffsetDiag = 1;
		yOffsetDiag = 1;  //checking diagonal right-below cell
		checkNearbyCells(arr, row, col, x, y, xOffset, yOffset, xOffsetDiag, yOffsetDiag);

		xOffset = -1;
		yOffset = 0;  //checking left cell 
		xOffsetDiag = -1;
		yOffsetDiag = -1;  //checking diagonal left-above cell
		checkNearbyCells(arr, row, col, x, y, xOffset, yOffset, xOffsetDiag, yOffsetDiag);

		xOffset = 0;
		yOffset = -1;  //checking above cell 
		xOffsetDiag = 1;
		yOffsetDiag = -1;  //checking diagonal right-above cell
		checkNearbyCells(arr, row, col, x, y, xOffset, yOffset, xOffsetDiag, yOffsetDiag);

		xOffset = 0;
		yOffset = 1;  //checking above cell 
		xOffsetDiag = -1;
		yOffsetDiag = 1;  //checking diagonal right-above cell
		checkNearbyCells(arr, row, col, x, y, xOffset, yOffset, xOffsetDiag, yOffsetDiag);
	}
	else if (!(*(*(arr + y) + x))->isFlag && !(*(*(arr + y) + x))->isOpen && !(*(*(arr + y) + x))->isBomb && (*(*(arr + y) + x))->content >= 1 && (*(*(arr + y) + x))->content <= 8) (*(*(arr + y) + x))->isOpen = true; //if open field with hint than just make it opened and don't open any other fields
	else if ((*(*(arr + y) + x))->isBomb)
	{
		(*(*(arr + y) + x))->isOpen = true;
		cout << "BOOOOOOOOOM!!!"; //создать функцию, которая будет выводить сообщение о взрыве на весь экран
		Sleep(500);
}
}

void checkNearbyCells(cell*** arr,  const int row, const int col, const int x, const int y, const int xOffset, const int yOffset, const int xOffsetDiag, const int yOffsetDiag)
{
	if ((x + xOffset) <= col - 1 && (x + xOffset) >= 0 && (y + yOffset) >= 0 && (y + yOffset) <= row - 1)  //if cell is in game field 
	{
		if (!(*(*(arr + y + yOffset) + x + xOffset))->isFlag && !(*(*(arr + y + yOffset) + x + xOffset))->isOpen && !(*(*(arr + y + yOffset) + x + xOffset))->isBomb)
		{
			if ((*(*(arr + y + yOffset) + x + xOffset))->content == 0) //and is empty 
			{
				openCell(arr, row, col, x + xOffset, y + yOffset);  //than call recursion function
			}
			else if ((*(*(arr + y + yOffset) + x + xOffset))->content >= 1 && (*(*(arr + y + yOffset) + x + xOffset))->content <= 8)  //if is hint 
			{
				(*(*(arr + y + yOffset) + x + xOffset))->isOpen = true;  //than just open that cell
			}
		}

		if ((y + yOffsetDiag) <= row - 1 && (y + yOffsetDiag) >= 0 && (x + xOffsetDiag) <= col - 1 && (x + xOffsetDiag) >= 0)  //if diagonal cell is in game field 
		{
			if (!(*(*(arr + y + yOffsetDiag) + x + xOffsetDiag))->isFlag && !(*(*(arr + y + yOffsetDiag) + x + xOffsetDiag))->isOpen && !(*(*(arr + y + yOffsetDiag) + x + xOffsetDiag))->isBomb)
			{
				if ((*(*(arr + y + yOffsetDiag) + x + xOffsetDiag))->content == 0)  //if diagonal right-below cell is empty 
				{
					openCell(arr, row, col, x + xOffsetDiag, y + yOffsetDiag); //than call recursion
				}
				else if ((*(*(arr + y + yOffsetDiag) + x + xOffsetDiag))->content >= 1 && (*(*(arr + y + yOffsetDiag) + x + xOffsetDiag))->content <= 8)  //if diagonal right-below cell is hint 
				{
					(*(*(arr + y + yOffsetDiag) + x + xOffsetDiag))->isOpen = true;  //than open it
				}
			}
		}
	}
}

void addScore(int& score, int& scoreCounter)
{
	scoreCounter++;
	int scoreCoef = scoreCounter / 5 + 1;
	score = score + 5 * scoreCoef;
}

bool winGame(const cell const* const* const* arr, const int row, const int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if ((*(*(arr + i) + j))->isBomb == true && (*(*(arr + i) + j))->isOpen == false && (*(*(arr + i) + j))->isFlag == false)  //if bombs left without flag and not opened
			{
				return false;  //than don't win
			}
		}
	}

	cout << "You win!" << endl;
	return true;
}