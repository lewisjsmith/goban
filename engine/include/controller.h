#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <queue> 
#include <sstream>
#include <string>
#include <algorithm>

#include "board.h"

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

enum MoveEvaluation {
    OK,
    FILLED,
    SUICIDE
};

std::string evaluationResponse(MoveEvaluation eval, unsigned int latestPos, Colour latestColour, std::set<unsigned int> deadStones) {
    switch(eval) {
        case MoveEvaluation::OK:
        {
            std::ostringstream oss("");
            for(auto start = deadStones.begin(); start != deadStones.end(); start++) {
                oss << " ";
                oss << static_cast<int>(*start);
            }
            if(deadStones.empty()) return std::string("ok") + " " + std::to_string(latestPos) + " " + std::to_string(static_cast<int>(latestColour));
            return std::string("ok") + " " + std::to_string(latestPos) + " " + std::to_string(static_cast<int>(latestColour)) + " " + "dead" + oss.str();
        }
        case MoveEvaluation::FILLED:
        {
            return std::string("invalid filled") + " " + std::to_string(latestPos);
        }
        case MoveEvaluation::SUICIDE:
        {
            return std::string("invalid suicide") + " " + std::to_string(latestPos) + " " + std::to_string(static_cast<int>(latestColour));
        }
    }
    return "evaluation error";
}

std::string evaluateBoard(Board& board, unsigned int latestPos, Colour latestColour) {
    std::vector<unsigned int> group = getGroup(board, latestPos, latestColour);
    std::vector<unsigned int> groupNeighbours = getGroupNeighbours(board, group, latestColour);
    std::vector<unsigned int> oppositeColourNeighbours = getOppositeColourNeighbours(board, groupNeighbours);
    std::vector<unsigned int> liberties = getLiberties(board, groupNeighbours);

    std::set<unsigned int> deadStones;

    if(oppositeColourNeighbours.empty()){
        return evaluationResponse(MoveEvaluation::OK, latestPos, latestColour, deadStones);
    } 

    Colour oppositeColour = board.get(oppositeColourNeighbours[0]);
    for(auto& n : oppositeColourNeighbours) {
        if(deadStones.count(n) == 1) continue;
        std::vector<unsigned int> oppositeGroup = getGroup(board, n, oppositeColour);
        std::vector<unsigned int> oppositeGroupNeighbours = getGroupNeighbours(board, oppositeGroup, oppositeColour);
        std::vector<unsigned int> oppositeGroupLiberties = getLiberties(board, oppositeGroupNeighbours);

        if(oppositeGroupLiberties.empty()) {
            std::sort(oppositeGroup.begin(), oppositeGroup.end());
            for(auto& stone : oppositeGroup) {
                board.removeStone(stone);
                deadStones.insert(stone);
            }
        }
    }

    if (deadStones.empty() && liberties.empty()) {
        board.set(latestPos, Colour::CLEAR);
        return evaluationResponse(MoveEvaluation::SUICIDE, latestPos, latestColour, deadStones);
    }

    return evaluationResponse(MoveEvaluation::OK, latestPos, latestColour, deadStones);
}

#endif 