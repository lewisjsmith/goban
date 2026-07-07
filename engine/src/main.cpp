#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "board.h"
#include "controller.h"

class Settings {
public:
    unsigned int size = 9;
    std::vector<Colour> saved_board{size * size, Colour::CLEAR};

    void resize(unsigned int newSize) {
        size = newSize;
        saved_board.assign(size * size, Colour::CLEAR);
    }
};

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

    Settings user_settings;

    if(!boardInit(user_settings.size, argc, argv)) return EXIT_FAILURE;

    Board* board = new Board(user_settings.size);

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
                if(board->placeStone(move, static_cast<Colour>(colour))) std::cout << evaluateBoard(*board, move, static_cast<Colour>(colour)) << std::endl;
                else std::cout << evaluationResponse(MoveEvaluation::FILLED, move, static_cast<Colour>(colour), {}) << std::endl;
            } else {
                std::cout << "invalid piece value" << std::endl;
            }
        }

        // Needs a differentiator between new game and reset to previously loaded
        if(action == "reset") {
            board = new Board(user_settings.size);
        }

        if(action == "resize") {
            std::string newSize;
            iss >> newSize;

            unsigned int newSizeInt = std::stoi(newSize);
            if(Board::isValidBoardSize(newSizeInt)) {
                user_settings.resize(newSizeInt);
                board = new Board(user_settings.size);    
                std::cout << "ok resize " << user_settings.size << std::endl;

            } else {
                std::cout << "invalid resize " << newSize << std::endl;
            }
        }

        if(action == "print") {
            board->printBoard();
        }

        if(action == "load") {
            std::string board_state;
            iss >> board_state;
            if(board->setBoardState(board_state)) {
                std::ostringstream oss;
                for(auto& c : board->getBoardState()) {
                    oss << static_cast<int>(c);
                }
                std::cout << "ok load " << oss.str() << std::endl;
                user_settings.saved_board = board->getBoardState();
            }
            
        }

        if(action == "quit") {
            break;
        }
    }

    return 0;
}

