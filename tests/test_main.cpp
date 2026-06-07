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

TEST(getNeighbourPosTest, OOB) {
    Board* board = new Board(9); 
    std::vector<unsigned int> a = {};
    std::vector<unsigned int> b = getNeighbourPos(*board, -1);
    std::vector<unsigned int> c = getNeighbourPos(*board, 81);
    ASSERT_EQ(a, b);
    ASSERT_EQ(a, c);

    board = new Board(13); 
    std::vector<unsigned int> d = {};
    std::vector<unsigned int> e = getNeighbourPos(*board, -1);
    std::vector<unsigned int> f = getNeighbourPos(*board, 169);
    ASSERT_EQ(d, e);
    ASSERT_EQ(d, f);

    board = new Board(19); 
    std::vector<unsigned int> g = {};
    std::vector<unsigned int> h = getNeighbourPos(*board, -1);
    std::vector<unsigned int> i = getNeighbourPos(*board, 361);
    ASSERT_EQ(g, h);
    ASSERT_EQ(g, i);
}

TEST(getNeighbourPosTest, BoundarySingleStone) {
    Board* board = new Board(9); 
    std::vector<unsigned int> a = getNeighbourPos(*board, 0);
    std::vector<unsigned int> b = {1, 9};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    board = new Board(9); 
    std::vector<unsigned int> c = getNeighbourPos(*board, 9);
    std::vector<unsigned int> d = {0, 10, 18};
    std::sort(c.begin(), c.end());
    std::sort(d.begin(), d.end());
    ASSERT_EQ(c, d);

    board = new Board(9); 
    std::vector<unsigned int> e = getNeighbourPos(*board, 80);
    std::vector<unsigned int> f = {71, 79};
    std::sort(e.begin(), e.end());
    std::sort(f.begin(), f.end());
    ASSERT_EQ(e, f);
}

TEST(getNeighbourPosTest, NonBoundarySingleStone) {
    Board* board = new Board(9); 
    std::vector<unsigned int> a = getNeighbourPos(*board, 11);
    std::vector<unsigned int> b = {2, 10, 12, 20};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);
}

TEST(getGroupTest, BoundarySingleStone) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    // Corners
    board = new Board(9); 
    board->set(0, 1);
    got = getGroup(*board, 0, 1);
    want = {0};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(8, 1);
    got = getGroup(*board, 8, 1);
    want = {8};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(72, 1);
    got = getGroup(*board, 72, 1);
    want = {72};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(80, 1);
    got = getGroup(*board, 80, 1);
    want = {80};
    ASSERT_EQ(got, want);

    // Sides
    board = new Board(9); 
    board->set(4, 1);
    got = getGroup(*board, 4, 1);
    want = {4};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(44, 1);
    got = getGroup(*board, 44, 1);
    want = {44};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(76, 1);
    got = getGroup(*board, 76, 1);
    want = {76};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(36, 1);
    got = getGroup(*board, 36, 1);
    want = {36};
    ASSERT_EQ(got, want);
}

TEST(getGroupTest, SingleStone) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(31, 1);
    got = getGroup(*board, 31, 1);
    want = {31};
    ASSERT_EQ(got, want);
}

TEST(getGroupTest, BoundaryGroup) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1);
    got = getGroup(*board, 0, 1);
    want = {0, 1, 9};
    ASSERT_EQ(got, want);
}

TEST(getGroupTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(22, 1);
    board->set(31, 1);
    board->set(40, 1);
    board->set(30, 1);
    board->set(32, 1);
    got = getGroup(*board, 31, 1);
    want = {22, 30, 31, 32, 40};
    std::sort(got.begin(), got.end());
    ASSERT_EQ(got, want);
}

TEST(getLibertiesTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1);
    got = getLiberties(*board, {0, 1, 9}, 1);
    want = {2, 10, 18};
    ASSERT_EQ(got, want);  
    

    board = new Board(9); 
    board->set(30, 1);
    board->set(31, 1);
    board->set(32, 1);
    board->set(39, 1);
    board->set(41, 1);
    board->set(42, 1);
    board->set(43, 1);
    board->set(48, 1);
    board->set(49, 1);
    board->set(50, 1);
    got = getLiberties(*board, {30, 31, 32, 39, 41, 42, 43, 48, 49, 50}, 1);
    std::sort(got.begin(), got.end());
    want = {21, 22, 23, 29, 33, 34, 38, 40, 44, 47, 51, 52, 57, 58, 59};
    ASSERT_EQ(got, want);   
}

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


// TEST(isAliveTest, AliveSingleStone) {
//     Board* board = new Board(9);
//     board->set(0, 1);
//     ASSERT_TRUE(isAlive(*board, 0));

//     board = new Board(9);
//     board->set(0, 1);
//     board->set(1, 2);
//     ASSERT_TRUE(isAlive(*board, 0));

//     board = new Board(9);
//     board->set(3, 1);
//     ASSERT_TRUE(isAlive(*board, 3));
// }

// TEST(isAliveTest, DeadSingleStone) {
//     Board* board = new Board(9);
//     board->set(0, 1);
//     board->set(1, 2);
//     board->set(9, 2);
//     ASSERT_FALSE(isAlive(*board, 0));

//     board = new Board(9);
//     board->set(11, 1);
//     board->set(2, 2);
//     board->set(10, 2);
//     board->set(12, 2);
//     board->set(20, 2);
//     ASSERT_FALSE(isAlive(*board, 11));
// }

// TEST(isAliveTest, AliveGroup) {
//     Board* board = new Board(9);
//     board->set(0, 1);
//     board->set(1, 1);
//     board->set(9, 1); 
//     ASSERT_TRUE(isAlive(*board, 0));

//     board = new Board(9);
//     board->set(13, 2);
//     board->set(14, 2);
//     board->set(4, 1);
//     board->set(5, 1);
//     board->set(12, 1);
//     board->set(15, 1);
//     board->set(22, 1);
//     ASSERT_TRUE(isAlive(*board, 13));
// }

// TEST(isAliveTest, DeadGroup) {
//     Board* board = new Board(9);
//     board->set(0, 1);
//     board->set(1, 1);
//     board->set(9, 1); 
//     board->set(2, 2);
//     board->set(10, 2);
//     board->set(18, 2);
//     ASSERT_FALSE(isAlive(*board, 0));

//     board = new Board(9);
//     board->set(13, 2);
//     board->set(14, 2);
//     board->set(4, 1);
//     board->set(5, 1);
//     board->set(12, 1);
//     board->set(15, 1);
//     board->set(22, 1);
//     board->set(23, 1);
//     ASSERT_FALSE(isAlive(*board, 13));
// }