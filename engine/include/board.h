#ifndef  BOARD_H
#define  BOARD_H

#include <vector>
#include <string>

enum class Colour : char {
    CLEAR = 0,
    BLACK = 1,
    WHITE = 2,
    OOB = 3
};

class Board {

    public:
    Board();
    Board(unsigned int);

    bool set(unsigned int, Colour); 
    Colour get(unsigned int) const;

    const std::vector<Colour>& getBoardState();
    bool setBoardState(const std::string&);

    void printBoard();

    unsigned int getWidth() const;
    bool setWidth(unsigned int);

    bool placeStone(unsigned int, Colour);
    bool removeStone(unsigned int);

    bool isOutOfBounds(unsigned int) const;

    static bool isValidBoardSize(unsigned int);
    static bool isValidBoardValue(Colour);

    private:
    std::vector<Colour> board;
    unsigned int width;

};

#endif