#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <utility>

#include "board.h"

Board::Board() : width(9) {
    board = std::vector<Colour>(9*9, Colour::CLEAR); 
    board_history[0] = board;
    board_history[1] = board;
}

Board::Board(unsigned int size) : width(size) {
    if(isValidBoardSize(size)) {
        board = std::vector<Colour>(size*size, Colour::CLEAR);
        board_history[0] = board;
        board_history[1] = board;
    }
    else {
        throw std::runtime_error("Invalid board size");
    }
}

bool Board::placeStone(unsigned int pos, Colour colour){
    Colour curr = get(pos);
    if(curr == Colour::CLEAR && (colour == Colour::BLACK || colour == Colour::WHITE)){ 
        return set(pos, colour);
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

const std::vector<Colour>& Board::getBoardState() {
    return board;
}

bool Board::setBoardState(const std::string& boardState) {
    unsigned int width = std::sqrt(boardState.size());
    if(!isValidBoardSize(width)) return false;
    for(unsigned int i = 0; i < boardState.size(); i++) {
        Colour stone_colour = static_cast<Colour>(boardState[i] - '0');
        if(stone_colour != Colour::CLEAR && stone_colour != Colour::BLACK && stone_colour != Colour::WHITE) return false;
        set(i, stone_colour); 
    }

    std::ostringstream oss;
    for(auto& c : getBoardState()) {
        oss << static_cast<int>(c);
    }
    std::cout << "ok load " << oss.str() << std::endl;

    return true;
}

void Board::printBoard() {
    std::ostringstream boardDebug;
    for(int i = 0; i < board.size(); i++) {
        boardDebug << (int)board[i] << " ";
        if((i+1)%width == 0) {
            boardDebug << "\n";
        }
    }
    std::cout << boardDebug.str() << std::endl;
}

unsigned int Board::getWidth() const {
    return width;
}

bool Board::setWidth(unsigned int size) {
    if(!isValidBoardSize(size)) return false;
    width = size;
    return true;
}

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

void Board::updateBoardHistory(std::vector<Colour> board_state) {
        board_history[0] = board_history[1];
        board_history[1] = board_state;
    }
