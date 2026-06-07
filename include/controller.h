#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include <vector>
#include <unordered_map>
#include <queue> 

#include <string>

std::vector<unsigned int> getNeighbourPos(Board& board, unsigned int pos) {
    
    std::vector<unsigned int> neighbours;

    unsigned int& width = board.width;

    if(pos > width*width - 1) return neighbours;

    unsigned int min = 0;
    unsigned int max = (width*width) - 1;
    
    if(pos > min){
        if(pos%width != 0) neighbours.push_back(pos - 1);
    } 
    if(pos < max){
        if(pos%(width - 1) != 0 || pos == 0) neighbours.push_back(pos + 1);
    }
    if(pos >= width) neighbours.push_back(pos - width);
    if(pos <= max - width) neighbours.push_back(pos + width);

    return neighbours;
}

// get liberties func ? 
/*
    Basically,
    Save the colour of the last move
    Check if that last move has opposite colour liberty stones
    Check if any of those opposite colour stones now have no liberties
        If not, kill groups
        Otherwise, check if the placed stone has no liberties
            If not, kill placed stone
        Else do nothing 
*/

std::vector<unsigned int> getGroup(Board& board, unsigned int pos, char colour) {

    std::vector<unsigned int> group = {};

    std::unordered_map<unsigned int, bool> seen = {{pos, true}};

    std::queue<unsigned int> queue;
    queue.push(pos);

    while(!queue.empty()){

        unsigned int curr = queue.front();
        queue.pop();
        group.push_back(curr);

        std::vector<unsigned int> neighbours = getNeighbourPos(board, curr);
        
        for(auto&n : neighbours){
            if(seen.count(n) != 0) continue;
            char neighbour_colour = board.get(n);
            if(neighbour_colour == colour) queue.push(n);
            seen[n] = true;
        }

    }

    return group;
}   

std::vector<unsigned int> getLiberties(Board& board, std::vector<unsigned int> group, char colour) {
    std::vector<unsigned int> liberties = {};
    std::unordered_map<unsigned int, bool> seen = {};

    for(auto& s : group){
        std::vector<unsigned int> neighbours = getNeighbourPos(board, s);
        for(auto& n : neighbours){
            if(seen.count(n) != 0) continue;
            if(board.get(n) != colour) liberties.push_back(n);
            seen[n] = true;
        }
    }

    return liberties;
}

// std::vector<unsigned int> isAlive(Board& board, unsigned int pos) {

//     char group_colour = board.get(pos);

//     std::unordered_map<unsigned int, bool> seen = {{pos, true}};
//     std::vector<unsigned int> seen_group = {pos};
//     std::queue<unsigned int> group_to_check;
//     group_to_check.push(pos);

//     while(group_to_check.size() > 0) {

//         unsigned int curr = group_to_check.front();
//         seen_group.push_back(curr);
//         group_to_check.pop();

//         std::vector<unsigned int> curr_neighbours = getNeighbourPos(board, curr);
//         for(auto& n : curr_neighbours){
//             if(seen.count(n) > 0) continue;
//             char curr_neighbour = board.get(n);
//             if(curr_neighbour == 0) return {};
//             if(curr_neighbour == group_colour) group_to_check.push(n);
//             seen[n] = true;
//         }

//     }

//     return false;

// }

// std::string checkForDeadGroups(Board& board) {

// }

#endif 