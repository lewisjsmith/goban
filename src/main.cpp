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

            iss >> colour >> move;

            /*
                Possible outcomes:
                    Empty OK -> bool TRUE
                    Empty but no liberties -> remove from board -> separation of set method and remove method.
                    Full space -> bool FALSE
            */

            if(board->set(std::stoi(move), std::stoi(colour))) {
                if(isAlive(*board, std::stoi(move))){
                    // append updates to this message such as a delete group nearby 
                    std::cout << "ok" << std::endl;
                } else {
                    // constant expressions for error strings 
                    // this one is for dead stones / suicide stones
                    board->set(std::stoi(move), 0);
                    std::cout << "invalid dead" << std::endl;
                }
            } else {
                std::cout << "invalid filled" << std::endl;
            }
        }

        if(cmd == "quit") {
            break;
        }
    }

    return 0;
}

