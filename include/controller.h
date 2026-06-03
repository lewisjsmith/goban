#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include <vector>
#include <unordered_map>
#include <queue> 

std::vector<unsigned int> getNeighbourPos(Board& board, unsigned int pos) {
    
    std::vector<unsigned int> neighbours;

    unsigned int& width = board.width;

    unsigned int min = 0;
    unsigned int max = width^2 - 1;
    
    if(pos > min) neighbours.push_back(pos - 1);
    if(pos < max) neighbours.push_back(pos + 1);
    
    if(pos >= width) neighbours.push_back(pos - width);
    if(pos <= max - width) neighbours.push_back(pos + width);

    return neighbours;

}

bool isAlive(Board& board, unsigned int pos) {

    char group_colour = board.get(pos);

    std::unordered_map<unsigned int, bool> seen = {{pos, true}};
    std::queue<unsigned int> group_to_check;
    group_to_check.push(pos);

    while(group_to_check.size() > 0) {

        unsigned int curr = group_to_check.front();
        group_to_check.pop();

        std::vector<unsigned int> curr_neighbours = getNeighbourPos(board, curr);
        for(auto& n : curr_neighbours){
            if(seen.count(n) > 0) continue;
            char curr_neighbour = board.get(n);
            if(curr_neighbour == 0) return true;
            if(curr_neighbour == group_colour) group_to_check.push(n);
            seen[n] = true;
        }

    }

    return false;

}

#endif 