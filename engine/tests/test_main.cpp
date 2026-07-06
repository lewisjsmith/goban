#include <vector>
#include <string>
#include <algorithm>

#include "gtest/gtest.h"

#include "board.h"
#include "controller.h"

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
    board->set(0, Colour::BLACK);
    got = getGroup(*board, 0, Colour::BLACK);
    want = {0};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(8, Colour::BLACK);
    got = getGroup(*board, 8, Colour::BLACK);
    want = {8};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(72, Colour::BLACK);
    got = getGroup(*board, 72, Colour::BLACK);
    want = {72};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(80, Colour::BLACK);
    got = getGroup(*board, 80, Colour::BLACK);
    want = {80};
    ASSERT_EQ(got, want);

    // Sides
    board = new Board(9); 
    board->set(4, Colour::BLACK);
    got = getGroup(*board, 4, Colour::BLACK);
    want = {4};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(44, Colour::BLACK);
    got = getGroup(*board, 44, Colour::BLACK);
    want = {44};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(76, Colour::BLACK);
    got = getGroup(*board, 76, Colour::BLACK);
    want = {76};
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(36, Colour::BLACK);
    got = getGroup(*board, 36, Colour::BLACK);
    want = {36};
    ASSERT_EQ(got, want);
}

TEST(getGroupTest, SingleStone) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(31, Colour::BLACK);
    got = getGroup(*board, 31, Colour::BLACK);
    want = {31};
    ASSERT_EQ(got, want);
}

TEST(getGroupTest, BoundaryGroup) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, Colour::BLACK);
    board->set(1, Colour::BLACK);
    board->set(9, Colour::BLACK);
    got = getGroup(*board, 0, Colour::BLACK);
    want = {0, 1, 9};
    ASSERT_EQ(got, want);
}

TEST(getGroupTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(22, Colour::BLACK);
    board->set(31, Colour::BLACK);
    board->set(40, Colour::BLACK);
    board->set(30, Colour::BLACK);
    board->set(32, Colour::BLACK);
    got = getGroup(*board, 31, Colour::BLACK);
    want = {22, 30, 31, 32, 40};
    std::sort(got.begin(), got.end());
    ASSERT_EQ(got, want);
}

TEST(getGroupNeighboursTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, Colour::BLACK);
    board->set(1, Colour::BLACK);
    board->set(9, Colour::BLACK);
    got = getGroupNeighbours(*board, {0, 1, 9}, Colour::BLACK);
    want = {2, 10, 18};
    ASSERT_EQ(got, want);  
    

    board = new Board(9); 
    board->set(30, Colour::BLACK);
    board->set(31, Colour::BLACK);
    board->set(32, Colour::BLACK);
    board->set(39, Colour::BLACK);
    board->set(41, Colour::BLACK);
    board->set(42, Colour::BLACK);
    board->set(43, Colour::BLACK);
    board->set(48, Colour::BLACK);
    board->set(49, Colour::BLACK);
    board->set(50, Colour::BLACK);
    got = getGroupNeighbours(*board, {30, 31, 32, 39, 41, 42, 43, 48, 49, 50}, Colour::BLACK);
    std::sort(got.begin(), got.end());
    want = {21, 22, 23, 29, 33, 34, 38, 40, 44, 47, 51, 52, 57, 58, 59};
    ASSERT_EQ(got, want);   
}

TEST(getLibertiesTest, Group) {
    Board* board;
    std::vector<unsigned int> got;
    std::vector<unsigned int> want;

    board = new Board(9); 
    board->set(0, Colour::BLACK);
    got = getLiberties(*board, {1, 9});
    want = {1, 9};
    std::sort(got.begin(), got.end());
    std::sort(want.begin(), want.end());
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(0, Colour::BLACK);
    board->set(1, Colour::WHITE);
    got = getLiberties(*board, {1, 9});
    want = {9};
    std::sort(got.begin(), got.end());
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(30, Colour::BLACK);
    board->set(31, Colour::BLACK);
    board->set(32, Colour::BLACK);
    board->set(39, Colour::BLACK);
    board->set(41, Colour::BLACK);
    board->set(42, Colour::BLACK);
    board->set(43, Colour::BLACK);
    board->set(48, Colour::BLACK);
    board->set(49, Colour::BLACK);
    board->set(50, Colour::BLACK);
    board->set(22, Colour::WHITE);
    board->set(44, Colour::WHITE);
    board->set(57, Colour::WHITE);
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
    board->set(0, Colour::BLACK);
    got = getOppositeColourNeighbours(*board, {1, 9});
    want = {};
    std::sort(got.begin(), got.end());
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(0, Colour::BLACK);
    board->set(1, Colour::WHITE);
    got = getOppositeColourNeighbours(*board, {1, 9});
    want = {1};
    std::sort(got.begin(), got.end());
    ASSERT_EQ(got, want);

    board = new Board(9); 
    board->set(30, Colour::BLACK);
    board->set(31, Colour::BLACK);
    board->set(32, Colour::BLACK);
    board->set(39, Colour::BLACK);
    board->set(41, Colour::BLACK);
    board->set(42, Colour::BLACK);
    board->set(43, Colour::BLACK);
    board->set(48, Colour::BLACK);
    board->set(49, Colour::BLACK);
    board->set(50, Colour::BLACK);
    board->set(22, Colour::WHITE);
    board->set(44, Colour::WHITE);
    board->set(57, Colour::WHITE);
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
    board->set(0, Colour::BLACK);
    board->set(1, Colour::BLACK);
    board->set(9, Colour::BLACK);
    board->set(10, Colour::BLACK);

    board->set(2, Colour::WHITE);
    board->set(11, Colour::WHITE);
    board->set(18, Colour::WHITE);
    board->set(19, Colour::WHITE); 

    got = removeDeadStones(*board, 19, Colour::WHITE);
    want = "ok 19 2 dead 0 1 9 10";

    ASSERT_EQ(got, want);

    board = new Board(9);
    board->set(30, Colour::BLACK);
    board->set(31, Colour::BLACK);
    board->set(32, Colour::BLACK);
    board->set(39, Colour::BLACK);
    board->set(41, Colour::BLACK);
    board->set(48, Colour::BLACK);
    board->set(49, Colour::BLACK);
    board->set(50, Colour::BLACK);

    board->set(21, Colour::WHITE);
    board->set(22, Colour::WHITE);
    board->set(23, Colour::WHITE);
    board->set(29, Colour::WHITE); 
    board->set(33, Colour::WHITE);
    board->set(38, Colour::WHITE);
    board->set(40, Colour::WHITE);
    board->set(42, Colour::WHITE);
    board->set(47, Colour::WHITE); 
    board->set(51, Colour::WHITE);
    board->set(57, Colour::WHITE);
    board->set(58, Colour::WHITE);
    board->set(59, Colour::WHITE); 

    got = removeDeadStones(*board, 40, Colour::WHITE);
    want = "ok 40 2 dead 30 31 32 39 41 48 49 50";

    ASSERT_EQ(got, want);

    board = new Board(9);
    board->set(20, Colour::BLACK);
    board->set(28, Colour::BLACK);
    board->set(38, Colour::BLACK);
    board->set(30, Colour::BLACK);
    board->set(22, Colour::BLACK);
    board->set(32, Colour::BLACK);
    board->set(40, Colour::BLACK);

    board->set(29, Colour::WHITE);
    board->set(31, Colour::WHITE);

    got = removeDeadStones(*board, 30 , Colour::BLACK);
    want = "ok 30 1 dead 29 31";

    ASSERT_EQ(got, want);

    board = new Board(9);
    board->set(30, Colour::BLACK);
    board->set(31, Colour::BLACK);
    board->set(32, Colour::BLACK);
    board->set(39, Colour::BLACK);
    board->set(41, Colour::BLACK);
    board->set(48, Colour::BLACK);
    board->set(49, Colour::BLACK);
    board->set(50, Colour::BLACK);

    board->set(21, Colour::WHITE);
    board->set(22, Colour::WHITE);
    board->set(23, Colour::WHITE);
    board->set(29, Colour::WHITE); 
    
    board->set(38, Colour::WHITE);
    board->set(40, Colour::WHITE);
    board->set(42, Colour::WHITE);
    board->set(47, Colour::WHITE); 
    board->set(51, Colour::WHITE);
    board->set(57, Colour::WHITE);
    board->set(58, Colour::WHITE);
    board->set(59, Colour::WHITE); 

    got = removeDeadStones(*board, 40, Colour::WHITE);
    want = "invalid suicide 40 2";

    ASSERT_EQ(got, want);
}