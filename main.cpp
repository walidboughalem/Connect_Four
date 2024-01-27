#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
const int ROWS = 6;
const int COLS = 7;
const int CELL_SIZE = 100;
const int BOARD_WIDTH = COLS * CELL_SIZE;
const int BOARD_HEIGHT = (ROWS + 1) * CELL_SIZE;

std::vector<std::vector<int>> board(ROWS, std::vector<int>(COLS, 0)); // 0: Empty, 1: Player 1, 2: Player 2
int currentPlayer = 1;
bool gameEnded = false;

bool checkForWin(int row, int col) {
    // Check horizontally
    for (int c = 0; c <= COLS - 4; ++c) {
        if (board[row][c] == currentPlayer &&
            board[row][c + 1] == currentPlayer &&
            board[row][c + 2] == currentPlayer &&
            board[row][c + 3] == currentPlayer) {
            return true;
        }
    }

    // Check vertically
    for (int r = 0; r <= ROWS - 4; ++r) {
        if (board[r][col] == currentPlayer &&
            board[r + 1][col] == currentPlayer &&
            board[r + 2][col] == currentPlayer &&
            board[r + 3][col] == currentPlayer) {
            return true;
        }
    }

    // Check diagonally (positive slope)
    for (int r = 0; r <= ROWS - 4; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            if (board[r][c] == currentPlayer &&
                board[r + 1][c + 1] == currentPlayer &&
                board[r + 2][c + 2] == currentPlayer &&
                board[r + 3][c + 3] == currentPlayer) {
                return true;
            }
        }
    }

    // Check diagonally (negative slope)
    for (int r = 3; r < ROWS; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            if (board[r][c] == currentPlayer &&
                board[r - 1][c + 1] == currentPlayer &&
                board[r - 2][c + 2] == currentPlayer &&
                board[r - 3][c + 3] == currentPlayer) {
                return true;
            }
        }
    }

    return false;
}

void dropPiece(int col) {
    if (gameEnded) {
        // Reset the game
        std::fill(board.begin(), board.end(), std::vector<int>(COLS, 0));
        currentPlayer = 1;
        gameEnded = false;
        return;
    }

    for (int row = ROWS - 1; row >= 0; --row) {
        if (board[row][col] == 0) {
            board[row][col] = currentPlayer;
            if (checkForWin(row, col)) {
                std::cout << "Player " << currentPlayer << " wins!" << std::endl;
                gameEnded = true;
            }
            currentPlayer = 3 - currentPlayer;
            break;
        }
    }
}

int main() {
    std::ofstream logFile("log.txt");
    logFile << "Error occurred." << std::endl;
    sf::RenderWindow window(sf::VideoMode(BOARD_WIDTH, BOARD_HEIGHT), "Connect Four");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    sf::Text startText("Click to Start", font, 30);
    startText.setPosition(BOARD_WIDTH / 2 - startText.getGlobalBounds().width / 2, BOARD_HEIGHT / 2 - startText.getGlobalBounds().height);

    sf::Text rulesText("Rules: Connect four pieces of your color in a row, column, or diagonal to win!", font, 20);
    rulesText.setPosition(BOARD_WIDTH / 2 - rulesText.getGlobalBounds().width / 2, startText.getPosition().y - 50);

    sf::Text restartText("Click to Restart", font, 30);
    restartText.setPosition(BOARD_WIDTH / 2 - restartText.getGlobalBounds().width / 2, BOARD_HEIGHT / 2 - restartText.getGlobalBounds().height);

    bool gameStarted = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed) {
                if (!gameStarted) {
                    gameStarted = true;
                    continue;
                }

                if (gameEnded) {
                    dropPiece(0);
                    continue;
                }

                int col = event.mouseButton.x / CELL_SIZE;
                dropPiece(col);
            }
        }

        window.clear(sf::Color::Blue);

        if (!gameStarted) {
            window.draw(startText);
            window.draw(rulesText);
        } else {
            for (int row = 0; row < ROWS; ++row) {
                for (int col = 0; col < COLS; ++col) {
                    sf::CircleShape circle(CELL_SIZE / 2 - 5);
                    circle.setFillColor(sf::Color::Black);
                    circle.setPosition(col * CELL_SIZE, (row + 1) * CELL_SIZE);
                    window.draw(circle);

                    if (board[row][col] == 1) {
                        sf::CircleShape piece(CELL_SIZE / 2 - 10);
                        piece.setFillColor(sf::Color::Red);
                        piece.setPosition(col * CELL_SIZE + 5, (row + 1) * CELL_SIZE + 5);
                        window.draw(piece);
                    } else if (board[row][col] == 2) {
                        sf::CircleShape piece(CELL_SIZE / 2 - 10);
                        piece.setFillColor(sf::Color::Yellow);
                        piece.setPosition(col * CELL_SIZE + 5, (row + 1) * CELL_SIZE + 5);
                        window.draw(piece);


                    }
                }
            }



            if (gameEnded) {
                window.draw(restartText);
            }
        }

        window.display();
    }

    return 0;
}
