#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include <vector>

std::vector<unsigned int> getNeighbourPos(Board& board, unsigned int pos) {
    
    std::vector<unsigned int> neighbours;

    unsigned int min = 0;
    unsigned int max = board.width^2 - 1;
    
    if(pos > min) neighbours.push_back(pos - 1);
    if(pos < max) neighbours.push_back(pos + 1);
    if(pos >= width) neighbours.push_back(pos - width);
    if(pos <= max - width) neighbours.push_back(pos + width);

    return neighbours;

}

#endif 