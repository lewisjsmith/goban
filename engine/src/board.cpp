#include "board.h"
#include <stdexcept>

Board::Board() : width(9) {
    board = std::vector<Colour>(9*9); 
}

Board::Board(unsigned int size) : width(size) {
    if(isValidBoardSize(size)) board = std::vector<Colour>(size*size);
    else {
        throw std::runtime_error("Invalid board size");
    }
}

bool Board::placeStone(unsigned int pos, Colour colour){
    Colour curr = get(pos);
    if(curr == Colour::CLEAR && (colour == Colour::BLACK || colour == Colour::WHITE)){ 
        set(pos, colour);
        return true;
    }
    return false;
};

bool Board::removeStone(unsigned int pos){
    return set(pos, Colour::CLEAR);
};

bool Board::set(unsigned int pos, Colour colour) {
    if(!isValidBoardValue(colour)) return false;
    if(isOutOfBounds(pos)) return false;
    board[pos] = colour;
    return true;
};

Colour Board::get(unsigned int pos) const {
    if(isOutOfBounds(pos)) return Colour::OOB;
    return board[pos];
};

bool Board::isOutOfBounds(unsigned int pos) const {
    return pos >= (width*width);
}

bool Board::isValidBoardSize(unsigned int size) {
    return (size == 9 || size == 13 || size == 19);
}

bool Board::isValidBoardValue(Colour colour) {
    return colour == Colour::BLACK ||
        colour == Colour::WHITE ||
        colour == Colour::CLEAR;
}
