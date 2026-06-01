#ifndef  BOARD_H
#define  BOARD_H

class Board {

    public:
    bool setValue(char); 
    char getValue(char);

    private:
    char board[9];
    
};

#endif