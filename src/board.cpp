#include "board.h"
#include <stdexcept>

Board::Board() : width(9) {
    board = std::vector<char>(9*9); 
}

Board::Board(unsigned int sz) : width(sz) {
    if(sz == 9 || sz == 13 || sz == 19) board = std::vector<char>(sz*sz);
    else {
        throw std::runtime_error("Invalid board size");
    }
}

void Board::set(unsigned int pos, const char c) {
    char pos_val = get(pos);
    if(pos_val == 0) board[pos] = c;
};

char Board::get(unsigned int pos) {
    if(pos < 0 || pos >= (width*width)) return 3;
    return board[pos];
};

// Assumes that 0 is empty, 1 is black, 2 is white, 3 is OOB