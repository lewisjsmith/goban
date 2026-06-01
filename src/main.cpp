#include "board.h"

// debug
#include <iostream>
using namespace std;

void simplePrint9x9(Board* board, int w) {
    for(int i = 0; i != w*w; i++) {
        if(i != 0 && i%w == 0) cout << endl;
        cout << int(board->board[i]);
    }
    cout << endl;
}

int main(int argc, char* argv[]) {

    int width = 9;
    Board* board = new Board(width);
    
    board->set(10, 1);
    simplePrint9x9(board, width);

    return 0;
}

