#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <regex> // Include the regex header

using namespace std;

// Global variables for width and height
const int WIDTH = 20;
const int HEIGHT = 15;

class Instructions {
public:
    virtual void ShowInstructions() {
        system("cls");
        cout << "Snake Game Instructions" << endl;
        cout << "Use the W-A-S-D keys to control the snake." << endl;
        cout << "Eat the fruit to grow longer and earn points." << endl;
        cout << "Avoid running into yourself." << endl;
        cout << "Press any key to start the game..." << endl;
        cout << "\n\n\n\n\n********** WARNING: DO NOT PLAY IF YOU HAVE EPILEPSY!! ***********" << endl;
        _getch(); // Wait for a key press before starting the game
    }

    virtual void ShowControls() {
        cout << "Controls: W-A-S-D keys" << endl;
    }

    virtual void ShowWarnings() {
        cout << "********** WARNING: DO NOT PLAY IF YOU HAVE EPILEPSY!! ***********" << endl;
    }
};

class GameOverScreen {
public:
    static void Show(int score) {
        ofstream outFile("game_over.txt"); // Open file for writing
        if (outFile.is_open()) {
            outFile << "Game Over!" << endl;
            outFile << "Your score: " << score << endl;
            outFile << "Press any key to exit...";
            outFile.close(); // Close the file
        }
        else {
            cout << "Unable to open file for writing!" << endl;
        }
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
        // Code to play background music (optional)
        cout << "Playing background music..." << endl;
    }

    void PlayGameOverSound() {
        // Code to play game over sound (optional)
        cout << "Playing game over sound..." << endl;
    }
};

class GraphicsSystem {
public:
    void InitializeGraphics() {
        // Code to initialize graphics system (optional)
        cout << "Initializing graphics system..." << endl;
    }

    void RenderFrame() {
        // Code to render game frame (optional)
        cout << "" << endl;
    }
};

// Example 1: Inheritance from Instructions
class CustomInstructions : public Instructions {
public:
    void ShowInstructions() override {
        Instructions::ShowInstructions();
        // Additional custom instructions
        cout << "Custom instructions for this game..." << endl;
    }

    void ShowControls() override {
        Instructions::ShowControls();
        // Additional custom controls
        cout << "Custom controls for this game..." << endl;
    }

    void ShowWarnings() override {
        Instructions::ShowWarnings();
        // Additional custom warnings
        cout << "Custom warnings for this game..." << endl;
    }
};

// Example 2: Inheritance from SoundSystem
class CustomSoundSystem : public SoundSystem {
public:
    // Add additional sound functionality if needed
};

// Example 3: Inheritance from GraphicsSystem
class CustomGraphicsSystem : public GraphicsSystem {
public:
    // Add additional graphics functionality if needed
};

class SnakeGame : public Instructions, public SoundSystem, public GraphicsSystem {
private:
    bool gameOver;
    int x, y, fruitX, fruitY, score;
    int tailX[100], tailY[100];
    int nTail;
    enum eDirection { STOP = 0, LEFT = 1, RIGHT = 2, UP = 4, DOWN = 8 }; // Using bitwise OR to set direction flags
    eDirection dir;
    Board board;
    void (SnakeGame::* InputFunction)(); // Function pointer for input

public:
    SnakeGame() {
        gameOver = false;
        dir = STOP;
        x = board.getWidth() / 2;
        y = board.getHeight() / 2;
        score = 0;
        nTail = 0;
        Setup();
        InputFunction = &SnakeGame::ProcessInput; // Set default input function
    }
    // Overloading the addition assignment operator (+=)
    SnakeGame& operator+=(const SnakeGame& other) {
        // Concatenate the tail of the other game with the tail of this game
        for (int i = 0; i < other.nTail; ++i) {
            tailX[nTail + i] = other.tailX[i];
            tailY[nTail + i] = other.tailY[i];
        }
        nTail += other.nTail;

        return *this;
    }

    // Overloading the subtraction assignment operator (-=)
    SnakeGame& operator-=(int length) {
        // Decrease the length of the tail by 'length'
        if (length >= nTail) {
            nTail = 0; // Reset the tail length if 'length' exceeds the current length
        }
        else {
            nTail -= length;
        }

        return *this;
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

    void ProcessInput() {
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

    void ProcessInput(int keyCode) {
        switch (keyCode) {
        case 75: // Left arrow key
            dir = LEFT;
            break;
        case 77: // Right arrow key
            dir = RIGHT;
            break;
        case 72: // Up arrow key
            dir = UP;
            break;
        case 80: // Down arrow key
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
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
            (this->*InputFunction)(); // Call the appropriate input function based on the input mode
            Logic();
            Sleep(150); // sleep(10);
            double fruitpiece = 3.5;
        }
    }

    void GameOverThread() {
        while (!gameOver) {
            // Keep waiting until the game is over
        }
        PlayGameOverSound();
        GameOverScreen::Show(score);
    }

    void SetInputMode(bool useArrowKeys) {
        if (useArrowKeys) {
            InputFunction = &SnakeGame::ProcessInput;
        }
        else {
            InputFunction = &SnakeGame::ProcessInput;
        }
    }

    // Operator overloading for concatenating two snake games
    SnakeGame operator+(const SnakeGame& other) const {
        // Create a new SnakeGame object
        SnakeGame combinedGame;

        // Copy the state of the current game
        combinedGame.gameOver = this->gameOver;
        combinedGame.x = this->x;
        combinedGame.y = this->y;
        combinedGame.fruitX = this->fruitX;
        combinedGame.fruitY = this->fruitY;
        combinedGame.score = this->score;
        combinedGame.nTail = this->nTail;
        combinedGame.dir = this->dir;
        for (int i = 0; i < this->nTail; ++i) {
            combinedGame.tailX[i] = this->tailX[i];
            combinedGame.tailY[i] = this->tailY[i];
        }

        // Concatenate the tail of the current game with the tail of the other game
        for (int i = 0; i < other.nTail; ++i) {
            combinedGame.tailX[combinedGame.nTail + i] = other.tailX[i];
            combinedGame.tailY[combinedGame.nTail + i] = other.tailY[i];
        }
        combinedGame.nTail += other.nTail;

        return combinedGame;
    }
};

// Template function to swap two values
template<typename T>
void Swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int main() {
    // Example 1: Inheritance from Instructions
    CustomInstructions customInstructions;
    customInstructions.ShowInstructions();
    customInstructions.ShowControls(); // Using the additional virtual function
    customInstructions.ShowWarnings(); // Using the additional virtual function

    // Example 2: Inheritance from SoundSystem
    CustomSoundSystem customSound;
    customSound.PlayBackgroundMusic();
    customSound.PlayGameOverSound();

    // Example 3: Inheritance from GraphicsSystem
    CustomGraphicsSystem customGraphics;
    customGraphics.InitializeGraphics();
    customGraphics.RenderFrame();

    // SnakeGame object
    SnakeGame game;
    game.RunGame();

    // Regular expression example
    string testString = "This is a Snake game.";
    regex pattern("Snake");

    if (regex_search(testString, pattern)) {
        cout << "The string contains the word 'Snake'." << endl;
    }
    else {
        cout << "The string does not contain the word 'Snake'." << endl;
    }

    return 0;
}



