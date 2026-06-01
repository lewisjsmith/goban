#ifndef  BOARD_H
#define  BOARD_H

#include <vector>

class Board {

    public:
    Board();
    Board(unsigned int);

    void set(char, const char); 
    char get(char);

    // private:
    std::vector<char> board;

};

#endif