#include "board.h"
#include "controller.h"
#include "gtest/gtest.h"

TEST(isAliveTest, AliveSingleStone) {
    Board* board = new Board(9);
    board->set(0, 1);
    ASSERT_TRUE(isAlive(*board, 0));

    board = new Board(9);
    board->set(0, 1);
    board->set(1, 2);
    ASSERT_TRUE(isAlive(*board, 0));

    board = new Board(9);
    board->set(3, 1);
    ASSERT_TRUE(isAlive(*board, 0));
}

TEST(isAliveTest, DeadSingleStone) {
    Board* board = new Board(9);
    board->set(0, 1);
    board->set(1, 2);
    board->set(9, 2);
    ASSERT_FALSE(isAlive(*board, 0));

    board = new Board(9);
    board->set(11, 1);
    board->set(2, 2);
    board->set(10, 2);
    board->set(12, 2);
    board->set(20, 2);
    ASSERT_FALSE(isAlive(*board, 11));
}

TEST(isAliveTest, AliveMultipleStones) {
    Board* board = new Board(9);
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1); 
    ASSERT_TRUE(isAlive(*board, 0));
}

TEST(isAliveTest, DeadMultipleStones) {

    /*
    
        0 1 2 3 4 5 6 7 8
        9 10 11 12 13 14 15 16 17
        18 19 20 21 22 23 24 25 26

    */

    Board* board = new Board(9);
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1); 
    board->set(2, 2);
    board->set(10, 2);
    board->set(18, 2);
    ASSERT_FALSE(isAlive(*board, 0));
}