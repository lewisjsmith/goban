#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

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

TEST(getNeighboursTest, OOB) {
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

TEST(getNeighboursTest, BoundarySingleStone9x9) {
    // Top left
    Board* board = new Board(9); 
    std::vector<unsigned int> a = getNeighbours(*board, 0);
    std::vector<unsigned int> b = {1, 9};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Top right
    board = new Board(9); 
    a = getNeighbours(*board, 8);
    b = {7, 17};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Bottom left
    board = new Board(9); 
    a = getNeighbours(*board, 72);
    b = {63, 73};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Bottom right
    board = new Board(9); 
    a = getNeighbours(*board, 80);
    b = {71, 79};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    board = new Board(9); 
    a = getNeighbours(*board, 9);
    b = {0, 10, 18};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);
}

/*
    13x13 Board Reference

    0    1    2    3    4    5    6    7    8    9    10   11   12
    13   14   15   16   17   18   19   20   21   22   23   24   25
    26   27   28   29   30   31   32   33   34   35   36   37   38
    39   40   41   42   43   44   45   46   47   48   49   50   51
    52   53   54   55   56   57   58   59   60   61   62   63   64
    65   66   67   68   69   70   71   72   73   74   75   76   77
    78   79   80   81   82   83   84   85   86   87   88   89   90
    91   92   93   94   95   96   97   98   99   100  101  102  103
    104  105  106  107  108  109  110  111  112  113  114  115  116
    117  118  119  120  121  122  123  124  125  126  127  128  129
    130  131  132  133  134  135  136  137  138  139  140  141  142
    143  144  145  146  147  148  149  150  151  152  153  154  155
    156  157  158  159  160  161  162  163  164  165  166  167  168

*/

TEST(getNeighboursTest, BoundarySingleStone13x13) {
    // Top left
    Board* board = new Board(13); 
    std::vector<unsigned int> a = getNeighbours(*board, 0);
    std::vector<unsigned int> b = {1, 13};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Top right
    board = new Board(13); 
    a = getNeighbours(*board, 12);
    b = {11, 25};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Bottom left
    board = new Board(13); 
    a = getNeighbours(*board, 156);
    b = {143, 157};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Bottom right
    board = new Board(13); 
    a = getNeighbours(*board, 168);
    b = {155, 167};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);
}

/*
    19x19 Board Reference

    0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18
    19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37
    38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53   54   55   56
    57   58   59   60   61   62   63   64   65   66   67   68   69   70   71   72   73   74   75
    76   77   78   79   80   81   82   83   84   85   86   87   88   89   90   91   92   93   94
    95   96   97   98   99   100  101  102  103  104  105  106  107  108  109  110  111  112  113
    114  115  116  117  118  119  120  121  122  123  124  125  126  127  128  129  130  131  132
    133  134  135  136  137  138  139  140  141  142  143  144  145  146  147  148  149  150  151
    152  153  154  155  156  157  158  159  160  161  162  163  164  165  166  167  168  169  170
    171  172  173  174  175  176  177  178  179  180  181  182  183  184  185  186  187  188  189
    190  191  192  193  194  195  196  197  198  199  200  201  202  203  204  205  206  207  208
    209  210  211  212  213  214  215  216  217  218  219  220  221  222  223  224  225  226  227
    228  229  230  231  232  233  234  235  236  237  238  239  240  241  242  243  244  245  246
    247  248  249  250  251  252  253  254  255  256  257  258  259  260  261  262  263  264  265
    266  267  268  269  270  271  272  273  274  275  276  277  278  279  280  281  282  283  284
    285  286  287  288  289  290  291  292  293  294  295  296  297  298  299  300  301  302  303
    304  305  306  307  308  309  310  311  312  313  314  315  316  317  318  319  320  321  322
    323  324  325  326  327  328  329  330  331  332  333  334  335  336  337  338  339  340  341
    342  343  344  345  346  347  348  349  350  351  352  353  354  355  356  357  358  359  360

*/

TEST(getNeighboursTest, BoundarySingleStone19x19) {

    // Top left
    Board* board = new Board(19); 
    std::vector<unsigned int> a = getNeighbours(*board, 0);
    std::vector<unsigned int> b = {1, 19};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Top right
    board = new Board(19); 
    a = getNeighbours(*board, 18);
    b = {17, 37};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Bottom left
    board = new Board(19); 
    a = getNeighbours(*board, 342);
    b = {323, 343};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);

    // Bottom right
    board = new Board(19); 
    a = getNeighbours(*board, 360);
    b = {341, 359};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);
}

TEST(getNeighboursTest, NonBoundarySingleStone) {
    Board* board = new Board(9); 
    std::vector<unsigned int> a = getNeighbours(*board, 11);
    std::vector<unsigned int> b = {2, 10, 12, 20};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ASSERT_EQ(a, b);
}

TEST(setTest, Inputs) {
    Board* board = new Board(9);
    bool a = board->set(-1, Colour::BLACK);
    ASSERT_EQ(a, false);

    a = board->set(81, Colour::BLACK); 
    ASSERT_EQ(a, false);

    a = board->set(0, Colour::OOB);
    ASSERT_EQ(a, false);

    a = board->set(0, Colour::CLEAR);
    ASSERT_EQ(a, true);

    a = board->set(0, Colour::BLACK);
    ASSERT_EQ(a, true);

    a = board->set(0, Colour::WHITE);
    ASSERT_EQ(a, true);

    a = board->set(0, static_cast<Colour>(4));
    ASSERT_EQ(a, false);
}

TEST(getTest, OOBInputs) {
    Board* board = new Board(9);
    Colour a = board->get(-1);
    ASSERT_EQ(a, Colour::OOB);

    a = board->get(81);
    ASSERT_EQ(a, Colour::OOB);
}

TEST(getTest, BoardUpdates) {
    Board* board = new Board(9);
    Colour a = board->get(50);
    ASSERT_EQ(a, Colour::CLEAR);

    board->set(30, Colour::BLACK);
    a = board->get(30);
    ASSERT_EQ(a, Colour::BLACK);

    board->set(30, Colour::WHITE);
    a = board->get(30);
    ASSERT_EQ(a, Colour::WHITE);

    board->set(30, Colour::CLEAR);
    a = board->get(30);
    ASSERT_EQ(a, Colour::CLEAR);
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

TEST(evaluateBoardTest, Group) {
    Board* board;
    std::string got;
    std::string want;

    board = new Board(9);
    board->set(0, Colour::BLACK);
    got = evaluateBoard(*board, 0, Colour::WHITE);
    want = "ok 0 2";

    board = new Board(9);
    board->set(0, Colour::BLACK);
    board->set(1, Colour::BLACK);
    board->set(9, Colour::BLACK);
    board->set(10, Colour::BLACK);

    board->set(2, Colour::WHITE);
    board->set(11, Colour::WHITE);
    board->set(18, Colour::WHITE);
    board->set(19, Colour::WHITE); 

    got = evaluateBoard(*board, 19, Colour::WHITE);
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

    got = evaluateBoard(*board, 40, Colour::WHITE);
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

    got = evaluateBoard(*board, 30 , Colour::BLACK);
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

    got = evaluateBoard(*board, 40, Colour::WHITE);
    want = "invalid suicide 40 2";

    ASSERT_EQ(got, want);
}

TEST(setBoardState, Set9x9) {
    // Happy path
    Board* board = new Board(9);
    std::string input = "012201120210102211001221012112020101220110202011202210102120011221001202010212110";
    board->setBoardState(input);
    std::vector<Colour> newBoardState = board->getBoardState();
    std::ostringstream oss("");
    for(auto& c : newBoardState) {
        oss << static_cast<int>(c);
    }
    ASSERT_EQ(input, oss.str());

    bool success = true;

    // OOB value
    board = new Board(9);
    input = "012201120210102211001221012112320101220110202011202210102120011221001202010212110";
    success = board->setBoardState(input);
    ASSERT_EQ(success, false);

    // Erroneous value number
    success = true;
    board = new Board(9);
    input = "012201120210102211001221012112020101220115202011202210102120011221001202010212110";
    success = board->setBoardState(input);
    ASSERT_EQ(success, false);

    // Erroneous value letter
    success = true;
    board = new Board(9);
    input = "01220112021010221100122101211202010122t110202011202210102120011221001202010212110";
    success = board->setBoardState(input);
    ASSERT_EQ(success, false);

    // Input too long
    success = true;
    board = new Board(9);
    input = "01220112021010221100122101211232010122011020201120221010212001122100120201021211012212";
    success = board->setBoardState(input);
    ASSERT_EQ(success, false);
}

TEST(koRule, Repetiton) {
    Board* board = new Board(9);
    std::string input = std::string("000000000") + \
                        std::string("000100000") + \
                        std::string("001010000") + \
                        std::string("002120000") + \
                        std::string("000200000") + \
                        std::string("000000000") + \
                        std::string("000000000") + \
                        std::string("000000000") + \
                        std::string("000000000");
    bool success = board->setBoardState(input);
    EXPECT_EQ(success, true);

    board->updateBoardHistory();
    
    board->placeStone(21, Colour::WHITE);
    board->removeStone(30);
    board->updateBoardHistory();
    EXPECT_EQ(isKoRepetition(*board), false);
    
    board->placeStone(30, Colour::BLACK);
    board->removeStone(21);
    ASSERT_EQ(isKoRepetition(*board), true);
    
    std::string got = evaluateBoard(*board, 30, Colour::BLACK);
    ASSERT_EQ(got, "invalid ko 30 1");
}
