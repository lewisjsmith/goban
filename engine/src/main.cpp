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

#include <typeinfo>

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
            if (Board::isValidBoardValue(static_cast<Colour>(colour))) {
                if(board->set(move, static_cast<Colour>(colour))) {

                    std::cout << removeDeadStones(*board, move, static_cast<Colour>(colour)) << std::endl;

                } else {
                    std::cout << "invalid filled " << move << std::endl;
                }
            }
        }

        if(action == "reset") {
            board = new Board(size);
        }

        if(action == "resize") {
            std::string newSize;
            iss >> newSize;

            unsigned int newSizeInt = std::stoi(newSize);

            if(Board::isValidBoardSize(newSizeInt)) {

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
            std::vector<Colour> boardstate = board->getBoardState();

            for(int i = 0; i < boardstate.size(); i++) {
                boardDebug << (int)boardstate[i] << " ";
                if((i+1)%board->getWidth() == 0) {
                   boardDebug << "\n";
                }
            }

            std::cout << boardDebug.str() << std::endl;
        }

        if(action == "load") {
            std::string board_state;
            iss >> board_state;
            
            for(int i = 0; i < board_state.size(); i++) {
                board->set(i, static_cast<Colour>(board_state[i])); 
            }

            std::cout << "ok load " << board_state << std::endl;
        }

        if(action == "quit") {
            break;
        }
    }

    return 0;
}

