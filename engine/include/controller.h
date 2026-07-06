#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue> 
#include <sstream>

#include <string>
#include <algorithm>

// getPositionsAroundStone
std::vector<unsigned int> getNeighbours(const Board& board, unsigned int pos) {

    if(board.isOutOfBounds(pos)) return {};
    
    std::vector<unsigned int> neighbours;
    const unsigned int& width = board.getWidth();

    unsigned int min = 0;
    unsigned int max = (width*width) - 1;
    
    // Left
    if(pos > min) if(pos%width != 0) neighbours.push_back(pos - 1);
    // Right 
    if(pos < max) if((pos-(width - 1))%width != 0 || pos == 0) neighbours.push_back(pos + 1);
    // Up
    if(pos >= width) neighbours.push_back(pos - width);
    // Down 
    if(pos <= max - width) neighbours.push_back(pos + width);

    return neighbours;
}

// getGroupGivenSingleStone
std::vector<unsigned int> getGroup(const Board& board, unsigned int pos, Colour colour) {

    if(board.isOutOfBounds(pos)) return {};
    if(colour == Colour::CLEAR) return {};

    std::vector<unsigned int> group;
    std::unordered_set<unsigned int> seen = {pos};
    std::queue<unsigned int> queue;

    queue.push(pos);

    while(!queue.empty()){
        unsigned int curr = queue.front();
        queue.pop();
        group.push_back(curr);

        std::vector<unsigned int> neighbours = getNeighbours(board, curr);
        
        for(auto&n : neighbours){
            if(seen.count(n) != 0) continue;
            Colour neighbour_colour = board.get(n);
            if(neighbour_colour == colour) queue.push(n);
            seen.insert(n);
        }
    }

    return group;
}   

// getGroupNeighboursGivenGroup
std::vector<unsigned int> getGroupNeighbours(const Board& board, std::vector<unsigned int> group, Colour colour) {
    if(group.empty()) return {};

    std::vector<unsigned int> groupNeighbours;
    std::unordered_set<unsigned int> seen;

    for(auto& s : group){
        std::vector<unsigned int> neighbours = getNeighbours(board, s);
        for(auto& n : neighbours){
            if(seen.count(n) != 0) continue;
            if(board.get(n) != colour) groupNeighbours.push_back(n);
            seen.insert(n);
        }
    }

    return groupNeighbours;
}

// getLibertiesFromNeighbourList - filters out liberties
std::vector<unsigned int> getLiberties(const Board& board, std::vector<unsigned int> groupNeighbours) {
    if(groupNeighbours.empty()) return {};

    std::vector<unsigned int> liberties;
    
    for(auto& n : groupNeighbours) {
        if(board.get(n) == Colour::CLEAR){
            liberties.push_back(n);
        }
    }

    return liberties;
}

// Filters out empty spaces from neighbours to just opposite colour stones
std::vector<unsigned int> getOppositeColourNeighbours(const Board& board, std::vector<unsigned int> groupNeighbours) {
    if(groupNeighbours.empty()) return {};

    std::vector<unsigned int> oppositeColourNeighbours;
    
    for(auto& n : groupNeighbours) {
        if(board.get(n) != Colour::CLEAR){
            oppositeColourNeighbours.push_back(n);
        }
    }

    return oppositeColourNeighbours;
}

std::string removeDeadStones(Board& board, unsigned int latestPos, Colour latestColour) {

    std::ostringstream result("");

    std::vector<unsigned int> group = getGroup(board, latestPos, latestColour);
    std::vector<unsigned int> groupNeighbours = getGroupNeighbours(board, group, latestColour);
    std::vector<unsigned int> oppositeColourNeighbours = getOppositeColourNeighbours(board, groupNeighbours);
    std::vector<unsigned int> liberties = getLiberties(board, groupNeighbours);

    // Happy path 
    if(oppositeColourNeighbours.empty()) return std::string("ok") + " " + std::to_string(latestPos) + " " + std::to_string(static_cast<int>(latestColour));
    
    Colour oppositeColour = board.get(oppositeColourNeighbours[0]);
    bool groupDeleted = false;

    for(auto& n : oppositeColourNeighbours) {
        std::vector<unsigned int> oppositeGroup = getGroup(board, n, oppositeColour);
        std::vector<unsigned int> oppositeGroupNeighbours = getGroupNeighbours(board, oppositeGroup, oppositeColour);
        std::vector<unsigned int> oppositeGroupLiberties = getLiberties(board, oppositeGroupNeighbours);

        // Delete opposite group as you have taken the last liberty 
        if(oppositeGroupLiberties.empty()) {
            if (std::string multiGroup = result.str(); multiGroup.find("dead") == std::string::npos){
                result << "dead";
            }
            std::sort(oppositeGroup.begin(), oppositeGroup.end());
            for(auto& stone : oppositeGroup) {
                result << " " << int(stone);
                board.removeStone(stone);
            }
            groupDeleted = true;
        }
    }

    // If the neighbouring group wasn't deleted and there are no liberties around the placed stone
    if (!groupDeleted && liberties.empty()) {
        result << "invalid suicide";
        result << " " << latestPos << " " << std::to_string(static_cast<int>(latestColour));
        board.set(latestPos, Colour::CLEAR);
        return result.str();
    }

    return std::string("ok") + " " + std::to_string(latestPos) + " " + std::to_string(static_cast<int>(latestColour)) + " " + result.str();
}

#endif 