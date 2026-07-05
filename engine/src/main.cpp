#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "board.h"
#include "controller.h"

bool boardInit(unsigned int& size, int argc, char* argv[]) {
    
    if (argc > 2) {
        return false;
    }

    if (argc == 2) {
        std::istringstream iss(argv[1]);
        if (!(iss >> size) || !iss.eof() || !Board::isValidBoardSize(size)) {
            std::cerr << "Invalid size: " << argv[1] << '\n';
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {

    unsigned int size = 9;
    if(!boardInit(size, argc, argv)) return EXIT_FAILURE;

    Board* board = new Board(size);

    std::string cmd;
    
    while(std::getline(std::cin, cmd)) {

        std::istringstream iss(cmd);
        std::string action;
        iss >> action;

        if(action == "play"){
            std::string colour_str;
            std::string move_str;

            iss >> move_str >> colour_str;

            int move = std::stoi(move_str);
            int colour = std::stoi(colour_str);

            if(board->set(move, colour)) {
                std::cout << removeDeadStones(*board, move, colour) << std::endl;
            } else {
                std::cout << "invalid filled " << move << std::endl;
            }
        }

        if(action == "reset") {
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

        if(action == "quit") {
            break;
        }
    }

    return 0;
}

