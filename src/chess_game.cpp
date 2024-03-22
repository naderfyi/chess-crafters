#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
#include <limits>

using namespace std;

enum class Color {
    White,
    Black
};

class piece {
protected:
    int x, y;
    Color color;
public:
    piece(Color color) : x(0), y(0), color(color) {}
    virtual ~piece() {}
    virtual bool move(int x0, int y0, int x1, int y1) = 0;
    virtual void print() = 0;
    Color getColor() const {
        return color;
    }
};

class piece_p : public piece {
public:
    piece_p(Color color) : piece(color) {}

    bool move(int x0, int y0, int x1, int y1) override {
        int direction = (color == Color::White) ? 1 : -1;
        
        bool forwardOne = x0 == x1 && (y1 - y0) == direction;
        
        bool forwardTwo = x0 == x1 && (y1 - y0) == 2 * direction && ((color == Color::White && y0 == 1) || (color == Color::Black && y0 == 6));
        
        return forwardOne || forwardTwo;
    }

    void print() override {
        cout << (color == Color::White ? "WP" : "BP");
    }
};

class piece_r : public piece {
public:
    piece_r(Color color) : piece(color) {}

    bool move(int x0, int y0, int x1, int y1) override {
        return x0 == x1 || y0 == y1;
    }

    void print() override {
        cout << (color == Color::White ? "WR" : "BR");
    }
};

class piece_n : public piece {
public:
    piece_n(Color color) : piece(color) {}

    bool move(int x0, int y0, int x1, int y1) override {
        return (abs(x1 - x0) == 2 && abs(y1 - y0) == 1) || (abs(x1 - x0) == 1 && abs(y1 - y0) == 2);
    }

    void print() override {
        cout << (color == Color::White ? "WN" : "BN");
    }
};

class piece_b : public piece {
public:
    piece_b(Color color) : piece(color) {}

    bool move(int x0, int y0, int x1, int y1) override {
        return abs(x1 - x0) == abs(y1 - y0);
    }

    void print() override {
        cout << (color == Color::White ? "WB" : "BB");
    }
};

class piece_q : public piece {
public:
    piece_q(Color color) : piece(color) {}

    bool move(int x0, int y0, int x1, int y1) override {
        return x0 == x1 || y0 == y1 || abs(x1 - x0) == abs(y1 - y0);
    }

    void print() override {
        cout << (color == Color::White ? "WQ" : "BQ");
    }
};

class piece_k : public piece {
public:
    piece_k(Color color) : piece(color) {}

    bool move(int x0, int y0, int x1, int y1) override {
        return abs(x1 - x0) <= 1 && abs(y1 - y0) <= 1;
    }

    void print() override {
        cout << (color == Color::White ? "WK" : "BK");
    }
};

template<typename T>
T* createPiece(Color color) {
    return new T(color);
}

class board {
public:
    piece *data[8][8];
    
    board() {
        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < 8; ++i) {
                data[j][i] = nullptr;
            }
        }

        for (int i = 0; i < 8; ++i) {
            data[1][i] = createPiece<piece_p>(Color::White);
            data[6][i] = createPiece<piece_p>(Color::Black);
        }

        data[0][0] = createPiece<piece_r>(Color::White);
        data[0][7] = createPiece<piece_r>(Color::White);
        data[7][0] = createPiece<piece_r>(Color::Black);
        data[7][7] = createPiece<piece_r>(Color::Black);

        data[0][1] = createPiece<piece_n>(Color::White);
        data[0][6] = createPiece<piece_n>(Color::White);
        data[7][1] = createPiece<piece_n>(Color::Black);
        data[7][6] = createPiece<piece_n>(Color::Black);

        data[0][2] = createPiece<piece_b>(Color::White);
        data[0][5] = createPiece<piece_b>(Color::White);
        data[7][2] = createPiece<piece_b>(Color::Black);
        data[7][5] = createPiece<piece_b>(Color::Black);

        data[0][3] = createPiece<piece_q>(Color::White);
        data[0][4] = createPiece<piece_k>(Color::White);
        data[7][3] = createPiece<piece_q>(Color::Black);
        data[7][4] = createPiece<piece_k>(Color::Black);
    }

    ~board() {
        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < 8; ++i) {
                delete data[j][i];
            }
        }
    }

    void print() {
        cout << "\n\n    A  B  C  D  E  F  G  H" << endl;
        cout << " +--------------------------+" << endl;
        for (int j = 0; j < 8; ++j) {
            cout << char('8' - j) << "| ";
            for (int i = 0; i < 8; ++i) {
                piece *c = data[j][i];
                if (c == nullptr) {
                    cout << " . ";
                } else {
                    cout << " ";
                    c->print();
                }
            }
            cout << " |";
            cout << endl;
        }
        cout << " +--------------------------+" << endl;
        cout << "    A  B  C  D  E  F  G  H" << endl;
    }

    void gameSetup(Color &playerColor, Color &aiColor, string &playerName) {
        cout << "\n\n==== 🤗 Welcome to Paris-Athens Chess Game! 🤗 ===" << endl;

        cout << "\nEnter your name: " << endl;
        cout << "> ";
        getline(cin, playerName);

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "An error occurred while reading your name. Please restart the game.\n";
            exit(1);
        }

        char colorChoice;
        bool validInput = false;
        do {
            cout << playerName << ", choose your color (W for White, B for Black): " << endl;
            cout << "> ";
            cin >> colorChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (colorChoice == 'W' || colorChoice == 'w') {
                playerColor = Color::White;
                aiColor = Color::Black;
                validInput = true;
            } else if (colorChoice == 'B' || colorChoice == 'b') {
                playerColor = Color::Black;
                aiColor = Color::White;
                validInput = true;
            } else {
                cout << "Invalid color choice. Please enter 'W' for White or 'B' for Black.\n";
            }
        } while (!validInput);

        cout << "You are playing as " << (playerColor == Color::White ? "White" : "Black") << ".\n";
    }

    bool isValidInput(const string& input) {
        return input.length() == 2 && input[0] >= 'A' && input[0] <= 'H' && input[1] >= '1' && input[1] <= '8';
    }

    string toChessNotation(int x, int y) {
        return string(1, 'A' + x) + to_string(8 - y);
    }

    bool isWithinBoard(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    bool isPathClear(int x0, int y0, int x1, int y1) const {
        int deltaX = (x1 == x0) ? 0 : (x1 > x0) ? 1 : -1;
        int deltaY = (y1 == y0) ? 0 : (y1 > y0) ? 1 : -1;

        int x = x0 + deltaX;
        int y = y0 + deltaY;

        while (x != x1 || y != y1) {
            if (data[y][x] != nullptr) {
                return false;
            }
            x += deltaX;
            y += deltaY;
        }

        return true;
    }

    bool validatePawnMove(piece* currentPiece, int startX, int startY, int endX, int endY, Color color) {
        piece* destinationPiece = data[endY][endX];
        bool isCaptureAllowed = destinationPiece != nullptr && destinationPiece->getColor() != color;

        int direction = (color == Color::White) ? 1 : -1;

        if (startX == endX && startY + direction == endY && destinationPiece == nullptr) {
            return true;
        }

        if (startX == endX && ((color == Color::White && startY == 1) || (color == Color::Black && startY == 6)) && startY + 2 * direction == endY) {
            return data[startY + direction][startX] == nullptr && destinationPiece == nullptr;
        }

        if (abs(startX - endX) == 1 && startY + direction == endY && isCaptureAllowed) {
            return true;
        }

        return false;
    }

    bool validateMove(piece* currentPiece, int startX, int startY, int endX, int endY, Color color) {
        if (!isWithinBoard(endX, endY)) return false;
        if (startX == endX && startY == endY) return false;

        piece* destinationPiece = data[endY][endX];
        bool isCaptureAllowed = destinationPiece != nullptr && destinationPiece->getColor() != color;

        piece_p* pawn = dynamic_cast<piece_p*>(currentPiece);
        if (pawn) {
            return validatePawnMove(currentPiece, startX, startY, endX, endY, color);
        }

        if (!currentPiece->move(startX, startY, endX, endY)) return false;

        piece_r* r = dynamic_cast<piece_r*>(currentPiece);
        piece_b* b = dynamic_cast<piece_b*>(currentPiece);
        piece_q* q = dynamic_cast<piece_q*>(currentPiece);
        if ((r || b || q) && !isPathClear(startX, startY, endX, endY)) {
            return false;
        }
        
        return destinationPiece == nullptr || isCaptureAllowed;
    }

    vector<pair<string, string>> findValidMovesForColor(Color color) {
        vector<pair<string, string>> validMoves;
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                piece* currentPiece = data[y][x];
                if (currentPiece != nullptr && currentPiece->getColor() == color) {
                    for (int toY = 0; toY < 8; ++toY) {
                        for (int toX = 0; toX < 8; ++toX) {
                            if (validateMove(currentPiece, x, y, toX, toY, color)) {
                                validMoves.emplace_back(toChessNotation(x, y), toChessNotation(toX, toY));
                            }
                        }
                    }
                }
            }
        }
        return validMoves;
    }

    bool kingExists(const Color kingColor) {
        int kingX = -1, kingY = -1;
        bool found = false;

        for (int y = 0; y < 8 && !found; ++y) {
            for (int x = 0; x < 8; ++x) {
                if (data[y][x] != nullptr && dynamic_cast<piece_k*>(data[y][x]) && data[y][x]->getColor() == kingColor) {
                    kingX = x;
                    kingY = y;
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        string kingPos = toChessNotation(kingX, kingY);

        if (kingX != -1) {
            return true; 
        } else { 
            return false;
        }
    }

};

bool executeMove(board& b, const Color& currentTurn, const string& from, const string& to) {
    if (!b.isValidInput(from) || !b.isValidInput(to)) {
        cout << "Invalid input format. Please use the format like 'A2 A3'." << endl;
        return false;
    }

    int fromX = toupper(from[0]) - 'A';
    int fromY = 8 - (from[1] - '0');
    int toX = toupper(to[0]) - 'A';
    int toY = 8 - (to[1] - '0');

    piece* p = b.data[fromY][fromX];
    if (p == nullptr) {
        cout << "There's no piece at " << from << ". Please try again." << endl;
        return false;
    }

    if (p->getColor() != currentTurn) {
        cout << "The piece at " << from << " doesn't belong to you. Please try again." << endl;
        return false;
    }

    vector<pair<string, string>> validMoves = b.findValidMovesForColor(currentTurn);
    string moveFrom = b.toChessNotation(fromX, fromY);
    string moveTo = b.toChessNotation(toX, toY);
    auto it = find_if(validMoves.begin(), validMoves.end(), [&](const pair<string, string>& move) {
        return move.first == moveFrom && move.second == moveTo;
    });

    if (it != validMoves.end()) {
        piece* destinationPiece = b.data[toY][toX];
        if (destinationPiece != nullptr) {
            delete destinationPiece;
        }
        b.data[toY][toX] = p;
        b.data[fromY][fromX] = nullptr;
        cout << (currentTurn == Color::White ? "\nWhite" : "\nBlack") << " moved " << moveFrom << " to " << moveTo << "." << endl;
        return true;
    } else {
        cout << "Invalid move for the selected piece. Please try again." << endl;
        return false;
    }
}

void playerMove(board& b, const Color& playerColor, const string& playerName, int& roundCounter) {
    cout << "\n\nTo quit, enter 'q'." << endl;
    cout << "\n\n" << playerName << ", enter your move (e.g., A2 A3): ";
    string from, to;
    cin >> from;
    if (from == "q") {
        cout << "\n\n❌ 🎮 Game over. " << playerName << " quit the game. 👋" << endl;
        cout << "\n🎉 AI wins! 🎉\n\n" << endl;
        exit(0);
    }
    cin >> to;
    if (!executeMove(b, playerColor, from, to)) {
        roundCounter--;
    }
}

void aiMove(board& b, const Color& aiColor, int& roundCounter) {
    vector<pair<string, string>> validMoves = b.findValidMovesForColor(aiColor);
    if (!validMoves.empty()) {
        int moveIndex = rand() % validMoves.size();
        auto selectedMove = validMoves[moveIndex];
        if (!executeMove(b, aiColor, selectedMove.first, selectedMove.second)) {
            roundCounter--;
        }
    } else {
        cout << "\nAI has no valid moves. Stalemate!" << endl;
        cout << "\n🎉 It's a draw! 🎉\n\n" << endl;
        exit(0);
    }
}

bool checkGameEnd(board& b, const Color& currentTurn) {
    Color opponent = (currentTurn == Color::White) ? Color::Black : Color::White;
    if (!b.kingExists(opponent)) {
        cout << "\n\n❌ 🎮 Game over. " << (currentTurn == Color::White ? "White" : "Black") << " wins! 🎉\n\n" << endl;
        return true;
    }
    return false;
}

int main() {
    try {
        board b;
        int roundCounter = 0;
        Color playerColor, aiColor;
        string playerName;
        srand(static_cast<unsigned int>(time(nullptr)));

        b.gameSetup(playerColor, aiColor, playerName);
        b.print();

        while (true) {
            Color currentTurn = (roundCounter % 2 == 0) ? playerColor : aiColor;

            if (currentTurn == playerColor) {
                playerMove(b, playerColor, playerName, roundCounter);
            } else {
                aiMove(b, aiColor, roundCounter);
            }

            b.print();

            if (checkGameEnd(b, currentTurn)) {
                break;
            }

            roundCounter++;
        }
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "An unknown error occurred." << endl;
        return 2;
    }

    return 0;
}
