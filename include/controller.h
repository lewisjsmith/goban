#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include <vector>
#include <unordered_map>
#include <queue> 
#include <sstream>

#include <string>
#include <algorithm>

// getPositionsAroundStone
std::vector<unsigned int> getNeighbours(Board& board, unsigned int pos) {
    
    std::vector<unsigned int> neighbours;

    unsigned int& width = board.width;

    if(pos > width*width - 1) return neighbours;

    unsigned int min = 0;
    unsigned int max = (width*width) - 1;
    
    if(pos > min){
        if(pos%width != 0) neighbours.push_back(pos - 1);
    } 
    if(pos < max){
        if((pos-(width - 1))%width != 0 || pos == 0) neighbours.push_back(pos + 1);
    }

    if(pos >= width) neighbours.push_back(pos - width);
    if(pos <= max - width) neighbours.push_back(pos + width);

    return neighbours;
}

// getGroupGivenSingleStone
std::vector<unsigned int> getGroup(Board& board, unsigned int pos, char colour) {

    std::vector<unsigned int> group = {};

    std::unordered_map<unsigned int, bool> seen = {{pos, true}};

    std::queue<unsigned int> queue;
    queue.push(pos);

    while(!queue.empty()){

        unsigned int curr = queue.front();
        queue.pop();
        group.push_back(curr);

        std::vector<unsigned int> neighbours = getNeighbours(board, curr);
        
        for(auto&n : neighbours){
            if(seen.count(n) != 0) continue;
            char neighbour_colour = board.get(n);
            if(neighbour_colour == colour) queue.push(n);
            seen[n] = true;
        }

    }

    return group;
}   

// getGroupNeighboursGivenGroup
std::vector<unsigned int> getGroupNeighbours(Board& board, std::vector<unsigned int> group, char colour) {
    std::vector<unsigned int> groupNeighbours = {};
    std::unordered_map<unsigned int, bool> seen = {};

    for(auto& s : group){
        std::vector<unsigned int> neighbours = getNeighbours(board, s);
        for(auto& n : neighbours){
            if(seen.count(n) != 0) continue;
            if(board.get(n) != colour) groupNeighbours.push_back(n);
            seen[n] = true;
        }
    }

    return groupNeighbours;
}

// getLibertiesFromNeighbourList
std::vector<unsigned int> getLiberties(Board& board, std::vector<unsigned int> groupNeighbours) {
    
    unsigned int emptyStoneCount;

    std::vector<unsigned int> liberties = {};
    
    for(auto& n : groupNeighbours) {
        if(board.get(n) == 0){
            emptyStoneCount++;
            liberties.push_back(n);
        }\
    }

    if(emptyStoneCount == 0) return {};
    return liberties;

}

std::vector<unsigned int> getOppositeColourNeighbours(Board& board, std::vector<unsigned int> groupNeighbours) {

    std::vector<unsigned int> oppositeColourNeighbours = {};
    
    for(auto& n : groupNeighbours) {
        if(board.get(n) != 0){
            oppositeColourNeighbours.push_back(n);
        }
    }

    return oppositeColourNeighbours;
}

// Consider swapping colours to 0 and 1 for easy falsy conversion
std::string removeDeadStones(Board& board, unsigned int latestPos, char latestColour) {

    std::ostringstream result("");

    std::vector<unsigned int> group = getGroup(board, latestPos, latestColour);
    std::vector<unsigned int> groupNeighbours = getGroupNeighbours(board, group, latestColour);
    std::vector<unsigned int> oppositeColourNeighbours = getOppositeColourNeighbours(board, groupNeighbours);
    std::vector<unsigned int> liberties = getLiberties(board, group);
    if(oppositeColourNeighbours.empty()) return result.str();
    
    char oppositeColour = board.get(oppositeColourNeighbours[0]);

    bool groupDeleted = false;

    for(auto& n : oppositeColourNeighbours) {

        std::vector<unsigned int> oppositeGroup = getGroup(board, n, oppositeColour);
        std::vector<unsigned int> oppositeGroupNeighbours = getGroupNeighbours(board, oppositeGroup, oppositeColour);
        std::vector<unsigned int> oppositeGroupLiberties = getLiberties(board, oppositeGroupNeighbours);

        if(oppositeGroupLiberties.empty()) {
            result << "dead";
            std::sort(oppositeGroup.begin(), oppositeGroup.end());
            for(auto& stone : oppositeGroup) {
                result << " " << int(stone);
                board.set(stone, 0);
            }
            groupDeleted = true;
        }
    }

    if (!groupDeleted) {
        result << "invalid";
        result << " " << latestPos;
        board.set(latestPos, 0);
    }

    return result.str();
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

#endif 