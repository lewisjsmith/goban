#include "board.h"
#include <stdexcept>

Board::Board() {
    board = std::vector<char>(9*9); 
}

Board::Board(unsigned int sz) {
    if(sz == 9 || sz == 13 || sz == 19) board = std::vector<char>(sz*sz);
    else {
        throw std::runtime_error("Invalid board size");
    }
}

void Board::set(char pos, const char c) {
    board[pos] = c;
};

char Board::get(char pos) {
    return board[pos];
};
