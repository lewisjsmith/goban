#include "board.h"
#include "controller.h"
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <algorithm>


/*
    9x9 Board Reference

    0   1   2   3   4   5   6   7   8
    9   10  11  12  13  14  15  16  17
    18  19  20  21  22  23  24  25  26
    27  28  29  30  31  32  33  34  35
    36  37  38  39  40  41  42  43  44
    45  46  47  48  49  50  51  52  53
    54  55  56  57  58  59  60  61  62
    63  64  65  66  67  68  69  70  71
    72  73  74  75  76  77  78  79  80
*/

TEST(getNeighbourPosTest, BoundarySingleStone) {
    Board* board = new Board(9); 
    board->set(0, 1);
    std::vector<unsigned int> a = getNeighbourPos(*board, 0);
    std::vector<unsigned int> b = {1, 9};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    board = new Board(9); 
    board->set(9, 1);
    std::vector<unsigned int> c = getNeighbourPos(*board, 9);
    std::vector<unsigned int> d = {0, 10, 18};
    std::sort(c.begin(), c.end());
    std::sort(d.begin(), d.end());
    ASSERT_EQ(c, d);
}

TEST(getNeighbourPosTest, NonBoundarySingleStone) {
    Board* board = new Board(9); 
    board->set(11, 2);
    std::vector<unsigned int> a = getNeighbourPos(*board, 11);
    std::vector<unsigned int> b = {2, 10, 12, 20};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);
}

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
    ASSERT_TRUE(isAlive(*board, 3));
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

TEST(isAliveTest, AliveGroup) {
    Board* board = new Board(9);
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1); 
    ASSERT_TRUE(isAlive(*board, 0));

    board = new Board(9);
    board->set(13, 2);
    board->set(14, 2);
    board->set(4, 1);
    board->set(5, 1);
    board->set(12, 1);
    board->set(15, 1);
    board->set(22, 1);
    ASSERT_TRUE(isAlive(*board, 13));
}

TEST(isAliveTest, DeadGroup) {
    Board* board = new Board(9);
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1); 
    board->set(2, 2);
    board->set(10, 2);
    board->set(18, 2);
    ASSERT_FALSE(isAlive(*board, 0));

    board = new Board(9);
    board->set(13, 2);
    board->set(14, 2);
    board->set(4, 1);
    board->set(5, 1);
    board->set(12, 1);
    board->set(15, 1);
    board->set(22, 1);
    board->set(23, 1);
    ASSERT_FALSE(isAlive(*board, 13));
}