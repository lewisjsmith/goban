#ifndef  BOARD_H
#define  BOARD_H

#include <vector>

class Board {

    Board();
    Board(unsigned int);

    public:
    void set(char, char); 
    char get(char);

    private:
    std::vector<char> board;

};

#endif