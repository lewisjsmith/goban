#include "board.h"
#include "controller.h"

#include <iostream>
#include <string>
#include <sstream>

int main(int argc, char* argv[]) {

    std::string cmd;
    Board* board = new Board();

    while(std::getline(std::cin, cmd)) {

        std::istringstream iss(cmd);
        std::string action;
        iss >> action;

        if(action == "play"){
            std::string colour;
            std::string move;

            iss >> move >> colour;

            bool stonePlaced = board->set(std::stoi(move), std::stoi(colour)); 

            if(stonePlaced) {
                std::cout << removeDeadStones(*board, std::stoi(move), std::stoi(colour)) << std::endl;
            } else {
                std::cout << "invalid filled " << std::stoi(move) << std::endl;
            }
        }

        if(action == "new") {
            board = new Board();
        }

        if(action == "quit") {
            break;
        }

        // simple board for testing
        for(int i = 0; i < ((board->width)*(board->width)); i++) {
            if(i % 9 == 0) std::cout << std::endl;
            std::cout << int(board->board[i]) << " ";
        }
        //
    }

    return 0;
}

