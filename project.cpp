#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

// Global variables for width and height
const int WIDTH = 20;
const int HEIGHT = 15;

class Instructions {
public:
    virtual void ShowInstructions() {
        system("cls");
        cout << "Snake Game Instructions" << endl;
        cout << "Use the arrow keys to control the snake." << endl;
        cout << "Eat the fruit to grow longer and earn points." << endl;
        cout << "Avoid running into the walls or yourself." << endl;
        cout << "Press any key to start the game..." << endl;
        cout << "\n\n\n\n\n********** WARNING: DO NOT PLAY IF YOU HAVE EPILEPSY!! ***********" << endl;
        _getch(); // Wait for a key press before starting the game
    }
};

class GameOverScreen {
public:
    static void Show(int score) {
        system("cls");
        cout << "Game Over!" << endl;
        cout << "Your score: " << score << endl;
        cout << "Press any key to exit...";
        _getch(); // Wait for a key press before exiting
    }
};

class Board {
public:
    int getWidth() { return WIDTH; }
    int getHeight() { return HEIGHT; }
};

class SoundSystem {
public:
    void PlayBackgroundMusic() {
        // Code to play background music
        cout << "Playing background music..." << endl;
    }

    void PlayGameOverSound() {
        // Code to play game over sound
        cout << "Playing game over sound..." << endl;
    }
};

class GraphicsSystem {
public:
    void InitializeGraphics() {
        // Code to initialize graphics system
        cout << "Initializing graphics system..." << endl;
    }

    void RenderFrame() {
        // Code to render game frame
        cout << "" << endl;
    }
};

class SnakeGame : public Instructions, public SoundSystem, public GraphicsSystem {
private:
    bool gameOver;
    int x, y, fruitX, fruitY, score;
    int tailX[100], tailY[100];
    int nTail;
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    eDirection dir;
    Board board;

public:
    SnakeGame() {
        gameOver = false;
        dir = STOP;
        x = board.getWidth() / 2;
        y = board.getHeight() / 2;
        score = 0;
        nTail = 0;
        Setup();
    }

    void Setup() {
        // Generate initial fruit coordinates
        bool fruitOnSnake = true;
        while (fruitOnSnake) {
            fruitX = rand() % board.getWidth();
            fruitY = rand() % board.getHeight();
            fruitOnSnake = false;
            // Check if fruit overlaps with the snake's body or head
            for (int i = 0; i < nTail; i++) {
                if (tailX[i] == fruitX && tailY[i] == fruitY) {
                    fruitOnSnake = true;
                    break;
                }
            }
            if (x == fruitX && y == fruitY) {
                fruitOnSnake = true;
                break;
            }
        }
    }

    void ShowInstructions() override {
        Instructions::ShowInstructions();
    }

    void Draw() {
        system("cls");
        for (int i = 0; i < board.getWidth() + 2; i++)
            cout << "#";
        cout << endl;

        for (int i = 0; i < board.getHeight(); i++) {
            for (int j = 0; j < board.getWidth(); j++) {
                if (j == 0)
                    cout << "#";
                if (i == y && j == x)
                    cout << "O";
                else if (i == fruitY && j == fruitX)
                    cout << "*";
                else {
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";
                            print = true;
                        }
                    }
                    if (!print)
                        cout << " ";
                }

                if (j == board.getWidth() - 1)
                    cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < board.getWidth() + 2; i++)
            cout << "#";
        cout << endl;
        cout << "Score:" << score << endl;
    }

    void Input() {
        if (_kbhit()) {
            switch (_getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
            }
        }
    }

    void Logic() {
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
        switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
        }
        if (x >= board.getWidth()) x = 0; else if (x < 0) x = board.getWidth() - 1;
        if (y >= board.getHeight()) y = 0; else if (y < 0) y = board.getHeight() - 1;

        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y)
                gameOver = true;

        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % board.getWidth();
            fruitY = rand() % board.getHeight();
            nTail++;
        }
    }

    void RunGame() {
        ShowInstructions();
        thread gameThread(&SnakeGame::GameLoop, this);
        thread gameOverThread(&SnakeGame::GameOverThread, this);
        gameThread.join();
        gameOverThread.join();
    }

    void GameLoop() {
        InitializeGraphics();
        PlayBackgroundMusic();
        while (!gameOver) {
            RenderFrame();
            Draw();
            Input();
            Logic();
            Sleep(150); // sleep(10);
        }
    }

    void GameOverThread() {
        while (!gameOver) {
            // Keep waiting until the game is over
        }
        PlayGameOverSound();
        GameOverScreen::Show(score);
    }
};

int main() {
    SnakeGame game; // SnakeGame object
    game.RunGame();
    return 0;
}



