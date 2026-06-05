#ifndef  BOARD_H
#define  BOARD_H

#include <vector>

class Board {

    public:
    Board();
    Board(unsigned int);

    bool set(unsigned int, const char); 
    char get(unsigned int);

    // private:
    std::vector<char> board;
    unsigned int width;

};

#endif