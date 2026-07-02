#include "board.h"
#include "controller.h"

#include <iostream>
#include <string>
#include <sstream>

#include <typeinfo>

int main(int argc, char* argv[]) {

    // To-do error checking here
    unsigned int size = 9;
    if(argc == 2) {
        std::istringstream iss( argv[1] );
        iss >> size;
    }

    std::string cmd;
    Board* board = new Board(size);

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

        if(action == "reset") {
            // To-do error checking here
            board = new Board(size);
        }

        if(action == "resize") {
            std::string newSize;
            iss >> newSize;

            unsigned int newSizeInt = std::stoi(newSize);

            if(newSizeInt == 9 || newSizeInt == 13 || newSizeInt == 19) {
                // To-do error checking here
                size = std::stoi(newSize);
                board = new Board(size);    
                
                std::cout << "ok resize " << newSize << std::endl;
            } else {
                std::cout << "invalid resize " << newSize << std::endl;
            }
            
        }

        if(action == "print") {
            
            std::ostringstream boardDebug;

            for(int i = 0; i < board->board.size(); i++) {
                boardDebug << (int)board->board[i] << " ";
                if((i+1)%board->width == 0) {
                   boardDebug << "\n";
                }
            }

            std::cout << boardDebug.str() << std::endl;
        }

        if(action == "load") {
            std::string board_state;
            iss >> board_state;
            
            for(int i = 0; i < board_state.size(); i++) {
                board->set(i, board_state[i] - '0'); 
            }

            std::cout << "ok load " << board_state << std::endl;
        }

        if(action == "quit") {
            break;
        }

        // simple board for testing
        // for(int i = 0; i < ((board->width)*(board->width)); i++) {
        //     if(i % 9 == 0) std::cout << std::endl;
        //     std::cout << int(board->board[i]) << " ";
        // }
        //
    }

    return 0;
}

