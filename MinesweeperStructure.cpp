//minesweeper game
#include<iostream>
#include<conio.h>
#include<iomanip>
#include<Windows.h>
#include<math.h>
using namespace std;

struct cell
{
	bool isBomb;
	bool isOpen;
	bool isFlag;
	short content;
};

struct user
{
	short score;
	short fieldSize;
	char* nickname;
	void showUser()
	{
		printf("User: %s Score: %d Size of field: %d x %d\n", nickname, score, fieldSize, fieldSize);
	}
};

template<typename T>
void retNum(T&, const T, const T);

void createCell(cell*&);

void deleteCell(cell*&);

void createCellArr2D(cell***&, const int, const int);

void deleteCellArr2D(cell***&, const int, const int);

template<typename T>
T getRand(const T, const T, const int);

void setBombs(cell***, const int, const int, int);

void setHints(cell***, const int, const int);

void showGameField(const cell* const* const* const, const int, const int, const int, const int, const int, int, const int);

void makeUserMove(cell***, int&, int&, const int, const int, int&, int&, int&, int&, int&, bool&);

void openCell(cell***, const int, const int, const int, const int);

void openNearbyCell(cell***, const int, const int, const int, const int, const int, const int);

void addScore(int&, int&);

bool winGame(cell const* const* const*, const int, const int);

void gameGreeting();

void showBoom();

void showYouWin();

bool offerSaveResult(const cell const* const* const*, const int, const int, const int, const int, const int, const int, const int);

void saveUserScore(const int, const int);

int main()
{
	system("mode con cols=80 lines=50");

	int row{};
	int col{};

	gameGreeting();

	cout << setw(60) << "Enter size of game field (from 5 to 40): ";
	retNum(row, 5, 40);
	col = row;

	int qttOfBombs{ row * 2 };
	int qttOfFlags = qttOfBombs;

	int qttOfMoves{ col * row };
	int lifes{ 3 };
	int score{};
	int scoreCounter{ -1 };
	bool openSavedGame{ false };

	int x{ col / 2 }; //x and y are users coordinates for cursor
	int y{ row / 2 };

	cell*** gameField = new cell**[row] {};
	createCellArr2D(gameField, row, col);

	setBombs(gameField, row, col, qttOfBombs);

	setHints(gameField, row, col);

	while (true)
	{
		if (!qttOfMoves || !lifes || winGame(gameField, row, col)) break;
		else
		{
			showGameField(gameField, x, y, row, col, qttOfMoves, lifes, score);
			makeUserMove(gameField, x, y, row, col, qttOfMoves, lifes, score, scoreCounter, qttOfFlags, openSavedGame);
		}
	}

	if (lifes && winGame(gameField, row, col))
	{
		if (offerSaveResult(gameField, x, y, row, col, qttOfMoves, lifes, score))
		{
			saveUserScore(score, row);
		}
	}
	else
	{
		cout << endl;
		cout << setw(45) << "You lose!" << endl;
	}

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
			arr = new cell * *[row] {};
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
				*(arr + i) = new cell * [col] {};
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
			delete[] * (arr + i);
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
	int fieldWidth = col;
	system("cls");

	cout << endl;;

	for (int i = 0; i < row; i++)
	{
		if (i == 0)
		{
			cout << setw((80 - col) / 2) << (char)201;
			while (fieldWidth--)
			{
				cout << (char)205;
			}
			cout << (char)187 << endl;
			fieldWidth = col;
		}


		for (int j = 0; j < col; j++)
		{
			if (j == 0) cout << setw((80 - col) / 2) << (char)186;
			if (i == y && j == x) cout << (char)206;
			else if ((*(*(arr + i) + j))->isFlag) cout << (char)16;
			else if (!(*(*(arr + i) + j))->isOpen) cout << (char)178;
			else if ((*(*(arr + i) + j))->isOpen && !(*(*(arr + i) + j))->isBomb && (*(*(arr + i) + j))->content >= 1 && (*(*(arr + i) + j))->content <= 8) cout << (*(*(arr + i) + j))->content;
			else if ((*(*(arr + i) + j))->isOpen && !(*(*(arr + i) + j))->isBomb && (*(*(arr + i) + j))->content == 0) cout << (char)0;
			else if ((*(*(arr + i) + j))->isOpen && (*(*(arr + i) + j))->isBomb) cout << (char)15;
			if (j == col - 1) cout << (char)186;
		}

		if (i == row - 1)
		{
			cout << endl;
			cout << setw((80 - col) / 2) << (char)200;
			while (fieldWidth--)
			{
				cout << (char)205;
			}
			cout << (char)188;
		}
		cout << endl;
	}

	cout << endl;
	cout << setw(32) << "Your score: " << score;
	cout << "\t\tLifes left: ";
	while (lifes--)
	{
		cout << (char)3;
	}
	cout << endl;
	cout << setw(60) << "Moves left: " << qttOfMoves << endl;
}

void makeUserMove(cell*** arr, int& x, int& y, const int row, const int col, int& qttOfMoves, int& lifes, int& score, int& scoreCounter, int& qttOfFlags, bool& openSavedGame)
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
		else if (qttOfFlags > 0)
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

	default: cout << "Error! Wrong operation code!" << endl;
	break;
	}
}

void openCell(cell*** arr, const int row, const int col, const int x, const int y)
{
	if (!(*(*(arr + y) + x))->isFlag && !(*(*(arr + y) + x))->isOpen && !(*(*(arr + y) + x))->isBomb && (*(*(arr + y) + x))->content == 0)  //if try to open empty field without bomb and without hint
	{
		(*(*(arr + y) + x))->isOpen = true;

		int xOffset{};  //offsets are int from "-1" to "1" for checking cells on the right, on the left, below, above selected cell and diagonal cells
		int yOffset{};

		xOffset = 1;
		yOffset = 0;  //checking right cell
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);
		xOffset = 1;
		yOffset = 1;  //checking diagonal right-below cell
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);

		xOffset = -1;
		yOffset = 0;  //checking left cell 
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);
		xOffset = -1;
		yOffset = -1;  //checking diagonal left-above cell
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);

		xOffset = 0;
		yOffset = -1;  //checking above cell 
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);
		xOffset = 1;
		yOffset = -1;  //checking diagonal right-above cell
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);

		xOffset = 0;
		yOffset = 1;  //checking above cell 
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);
		xOffset = -1;
		yOffset = 1;  //checking diagonal right-above cell
		openNearbyCell(arr, row, col, x, y, xOffset, yOffset);
	}
	else if (!(*(*(arr + y) + x))->isFlag && !(*(*(arr + y) + x))->isOpen && !(*(*(arr + y) + x))->isBomb &&
		(*(*(arr + y) + x))->content >= 1 && (*(*(arr + y) + x))->content <= 8) (*(*(arr + y) + x))->isOpen = true; //if open field with hint than just make it opened and don't open any other fields
	else if ((*(*(arr + y) + x))->isBomb)
	{
		(*(*(arr + y) + x))->isOpen = true;
		showBoom();
	}
}

void openNearbyCell(cell*** arr, const int row, const int col, const int x, const int y, const int xOffset, const int yOffset)
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
	return true;
}

void gameGreeting()
{
	cout << endl;
	cout << setw(75) << "___  ________ _   _  _____ _____  _    _ _____ ___________ ___________ " << endl;
	cout << setw(75) << "|  \\/  |_   _| \\ | ||  ___/ ___| | |  | |  ___|  ___| ___ \\  ___| ___ \\" << endl;
	cout << setw(75) << "| .  . | | | |  \\| || |__ \\ `--. | |  | | |__ | |__ | |_/ / |__ | |_/ /" << endl;
	cout << setw(74) << "| |\\/| | | | | . ` ||  __| `--. \\| |/\\| |  __||  __|| __/ |  __||    /" << endl;
	cout << setw(74) << "| |  | |_| |_| |\\  || |___/\\__/ /\\  /\\  / |___| |___| |   | |___| |\\ \\" << endl;
	cout << setw(75) << "\\_|  |_/\\___/\\_| \\_/\\____/\\____/  \\/  \\/\\____/\\____/\\_|   \\____/\\_| \\_|" << endl;
	cout << endl;
}

void showBoom()
{
	system("cls");
	cout << endl;
	cout << endl;
	cout << setw(60) << "______  _____  ________  ___  _   _   _ " << endl;
	cout << setw(60) << "| ___ \\|  _  ||  _  |  \\/  | | | | | | |" << endl;
	cout << setw(60) << "| |_/ /| | | || | | | .  . | | | | | | |" << endl;
	cout << setw(60) << "| ___ \\| | | || | | | |\\/| | | | | | | |" << endl;
	cout << setw(60) << "| |_/ /\\ \\_/ /\\ \\_/ / |  | | |_| |_| |_|" << endl;
	cout << setw(60) << "\\____/  \\___/  \\___/\\_|  |_/ (_) (_) (_)" << endl;
	Sleep(500);
}

void showYouWin()
{
	cout << endl;
	cout << endl;
	cout << setw(65) << "__   _______ _   _   _    _ _____ _   _   _ " << endl;
	cout << setw(65) << "\\ \\ / /  _  | | | | | |  | |_   _| \\ | | | |" << endl;
	cout << setw(65) << " \\ V /| | | | | | | | |  | | | | |  \\| | | |" << endl;
	cout << setw(65) << "  \\ / | | | | | | | | |/\\| | | | | . ` | | |" << endl;
	cout << setw(65) << "  | | \\ \\_/ / |_| | \\  /\\  /_| |_| |\\  | |_|" << endl;
	cout << setw(65) << "  \\_/  \\___/ \\___/   \\/  \\/ \\___/\\_| \\_/ (_)" << endl;
}

bool offerSaveResult(const cell const* const* const* arr, const int x, const int y, const int row, const int col, const int qttOfMoves, const int lifes, const int score)
{
	cout << endl;

	bool userChoise = false;
	char symb{};
	while (true)
	{
		system("cls");
		showGameField(arr, x, y, row, col, qttOfMoves, lifes, score);
		showYouWin();
		cout << endl;
		cout << setw(75) << "Wish to save your name in Minesweeper Hall of Fame for descendants?\n" << endl;

		if (userChoise)
		{
			cout << setw(44) << ">  Yes  <" << endl;
			cout << setw(44) << "   No    " << endl;
		}
		else
		{
			cout << setw(44) << "   Yes   " << endl;
			cout << setw(44) << ">  No   <" << endl;
		}

		symb = _getch();
		if (symb == 'w' || symb == 's')
		{
			if (userChoise == true) userChoise = false;
			else if (userChoise == false) userChoise = true;
		}
		else if (symb == ' ')
		{
			return userChoise;
		}
	}
}

void saveUserScore(const int score, const int row)
{
	FILE* f{ nullptr };
	fopen_s(&f, "files/minesweeperHallOfFame.txt", "a");
	if (!f)
	{
		printf("File was not found!\n");
		return;
	}
	else
	{
		user currentUser{};
		currentUser.nickname = new char[50]{ '\0' };
		printf("Enter your nickname (without spaces):\n");
		fscanf_s(stdin, "%s", currentUser.nickname, 50);
		scanf_s("%*c");

		currentUser.score = score;

		currentUser.fieldSize = row;

		fprintf(f, "%s %d %d\n", currentUser.nickname, currentUser.score, currentUser.fieldSize);
		printf("Your score was successfully saved!\n");

		delete[] currentUser.nickname;
	}
	fclose(f);
}

void openLastSavedGame(cell***& oldGameField, int& row, int& col)
{
	FILE* fb = nullptr;
	fopen_s(&fb, "files/lastSavedGame.bin", "rb");

	if (!fb)
	{
		printf("Error! File was not found.\n");
	}
	else
	{
		fseek(fb, 0, SEEK_END);
		int size = ftell(fb);
		int fieldSqr = size / sizeof(cell);
		fseek(fb, 0, SEEK_SET);

		int rowNew{};
		int colNew{};
		rowNew = (int)sqrt(fieldSqr);
		colNew = rowNew;
		cell*** newGameField = nullptr;
		createCellArr2D(newGameField, rowNew, colNew);

		fread_s(newGameField, size, sizeof(cell), fieldSqr, fb);

		deleteCellArr2D(oldGameField, row, col);
		oldGameField = newGameField;
		row = rowNew;
		col = colNew;

		cout << setw(25) << "Your saved game successfully opened!" << endl;
		fclose(fb);
	}
}

template<typename T>
void retNum(T& n, const T min, const T max)
{
	while (true)
	{
		cin >> n;
		if (!cin)
		{
			cout << "Enter number again: ";
			cin.clear();
			while (cin.get() != '\n');
		}
		else if (n < min || n > max)
		{
			cout << "Enter number between " << min << " and " << max << ": ";
			continue;
		}
		else
		{
			cin.ignore(10, '\n');
			return;
		}
	}
}
