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

// SET METHOD TEST NEEDED

TEST(getNeighbourTest, OOB) {
    Board* board = new Board(9); 
    std::vector<unsigned int> a = {};
    std::vector<unsigned int> b = getNeighbours(*board, -1);
    std::vector<unsigned int> c = getNeighbours(*board, 81);
    ASSERT_EQ(a, b);
    ASSERT_EQ(a, c);

    board = new Board(13); 
    std::vector<unsigned int> d = {};
    std::vector<unsigned int> e = getNeighbours(*board, -1);
    std::vector<unsigned int> f = getNeighbours(*board, 169);
    ASSERT_EQ(d, e);
    ASSERT_EQ(d, f);

    board = new Board(19); 
    std::vector<unsigned int> g = {};
    std::vector<unsigned int> h = getNeighbours(*board, -1);
    std::vector<unsigned int> i = getNeighbours(*board, 361);
    ASSERT_EQ(g, h);
    ASSERT_EQ(g, i);
}

TEST(getNeighbourTest, BoundarySingleStone) {
    Board* board = new Board(9); 
    std::vector<unsigned int> a = getNeighbours(*board, 0);
    std::vector<unsigned int> b = {1, 9};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    board = new Board(9); 
    std::vector<unsigned int> c = getNeighbours(*board, 9);
    std::vector<unsigned int> d = {0, 10, 18};
    std::sort(c.begin(), c.end());
    std::sort(d.begin(), d.end());
    ASSERT_EQ(c, d);

    board = new Board(9); 
    std::vector<unsigned int> e = getNeighbours(*board, 80);
    std::vector<unsigned int> f = {71, 79};
    std::sort(e.begin(), e.end());
    std::sort(f.begin(), f.end());
    ASSERT_EQ(e, f);
}

TEST(getNeighboursTest, NonBoundarySingleStone) {
    Board* board = new Board(9); 
    std::vector<unsigned int> a = getNeighbours(*board, 11);
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

TEST(getGroupNeighboursTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1);
    got = getGroupNeighbours(*board, {0, 1, 9}, 1);
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
    got = getGroupNeighbours(*board, {30, 31, 32, 39, 41, 42, 43, 48, 49, 50}, 1);
    std::sort(got.begin(), got.end());
    want = {21, 22, 23, 29, 33, 34, 38, 40, 44, 47, 51, 52, 57, 58, 59};
    ASSERT_EQ(got, want);   
}

TEST(getLibertiesTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, 1);
    got = getLiberties(*board, {1, 9});
    want = {1, 9};
    std::sort(got.begin(), got.end());
    std::sort(want.begin(), want.end());
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(0, 1);
    board->set(1, 2);
    got = getLiberties(*board, {1, 9});
    want = {9};
    std::sort(got.begin(), got.end());
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
    board->set(22, 2);
    board->set(44, 2);
    board->set(57, 2);
    got = getLiberties(*board, {21, 22, 23, 29, 33, 34, 38, 40, 44, 47, 51, 52, 57, 58, 59});
    std::sort(got.begin(), got.end());
    want = {21, 23, 29, 33, 34, 38, 40, 47, 51, 52, 58, 59};
    ASSERT_EQ(got, want); 
}

TEST(getOppositeColourNeighboursTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, 1);
    got = getOppositeColourNeighbours(*board, {1, 9});
    want = {};
    std::sort(got.begin(), got.end());
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(0, 1);
    board->set(1, 2);
    got = getOppositeColourNeighbours(*board, {1, 9});
    want = {1};
    std::sort(got.begin(), got.end());
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
    board->set(22, 2);
    board->set(44, 2);
    board->set(57, 2);
    got = getOppositeColourNeighbours(*board, {21, 22, 23, 29, 33, 34, 38, 40, 44, 47, 51, 52, 57, 58, 59});
    std::sort(got.begin(), got.end());
    want = {22, 44, 57};
    ASSERT_EQ(got, want);    
}

TEST(removeDeadStonesTest, Group) {
    Board* board;
    std::string got;
    std::string want;

    board = new Board(9);
    board->set(0, 1);
    board->set(1, 1);
    board->set(9, 1);
    board->set(10, 1);

    board->set(2, 2);
    board->set(11, 2);
    board->set(18, 2);
    board->set(19, 2); 

    got = removeDeadStones(*board, 19, 2);
    want = "ok 19 2 dead 0 1 9 10";

    ASSERT_EQ(got, want);

    board = new Board(9);
    board->set(30, 1);
    board->set(31, 1);
    board->set(32, 1);
    board->set(39, 1);
    board->set(41, 1);
    board->set(48, 1);
    board->set(49, 1);
    board->set(50, 1);

    board->set(21, 2);
    board->set(22, 2);
    board->set(23, 2);
    board->set(29, 2); 
    board->set(33, 2);
    board->set(38, 2);
    board->set(40, 2);
    board->set(42, 2);
    board->set(47, 2); 
    board->set(51, 2);
    board->set(57, 2);
    board->set(58, 2);
    board->set(59, 2); 

    got = removeDeadStones(*board, 40, 2);
    want = "ok 40 2 dead 30 31 32 39 41 48 49 50";

    ASSERT_EQ(got, want);

    board = new Board(9);
    board->set(20, 1);
    board->set(28, 1);
    board->set(38, 1);
    board->set(30, 1);
    board->set(22, 1);
    board->set(32, 1);
    board->set(40, 1);

    board->set(29, 2);
    board->set(31, 2);

    got = removeDeadStones(*board, 30 , 1);
    want = "ok 30 1 dead 29 31";

    ASSERT_EQ(got, want);

    board = new Board(9);
    board->set(30, 1);
    board->set(31, 1);
    board->set(32, 1);
    board->set(39, 1);
    board->set(41, 1);
    board->set(48, 1);
    board->set(49, 1);
    board->set(50, 1);

    board->set(21, 2);
    board->set(22, 2);
    board->set(23, 2);
    board->set(29, 2); 
    
    board->set(38, 2);
    board->set(40, 2);
    board->set(42, 2);
    board->set(47, 2); 
    board->set(51, 2);
    board->set(57, 2);
    board->set(58, 2);
    board->set(59, 2); 

    got = removeDeadStones(*board, 40, 2);
    want = "invalid suicide 40 2";

    ASSERT_EQ(got, want);
}