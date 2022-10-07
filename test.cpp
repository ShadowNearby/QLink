#include "test.h"

QVector<QVector<int>> map = { { 1, 1, 0 }, { 1, 1, 1 }, { 0, 0, 0 } };
/*
 *
 *          0   1   2
 *
 *      0   +   +
 *
 *      1   +   +   +
 *
 *      2
 *
 */
Test::Test()
{
    game = new Game(3);
}
/*
 *
 *          0   1   2
 *
 *      0   +---+
 *
 *      1   +   +   +
 *
 *      2
 *
 */
void Test::testCase1()
{
    QPair<int, int> block1(0, 0), block2(0, 1);
    QCOMPARE(game->test(map, block1, block2), true);
}
/*
 *
 *          0   1   2
 *
 *      0   +   +
 *          |
 *      1   +   +   +
 *
 *      2
 *
 */
void Test::testCase2()
{
    QPair<int, int> block1(0, 0), block2(1, 0);
    QCOMPARE(game->test(map, block1, block2), true);
}
/*
 *
 *          0   1   2
 *
 *      0   +   +---|
 *                  |
 *      1   +   +   +
 *
 *      2
 *
 */
void Test::testCase3()
{
    QPair<int, int> block1(1, 2), block2(0, 1);
    QCOMPARE(game->test(map, block1, block2), true);
}
/*
 *
 *          0   1   2
 *
 *      0   +   +
 *
 *      1   +   +   +
 *          |       |
 *      2   |-------|
 *
 */
void Test::testCase4()
{
    QPair<int, int> block1(1, 0), block2(1, 2);
    QCOMPARE(game->test(map, block1, block2), true);
}
/*
 *
 *          0   1   2
 *
 *      0   +*  +
 *
 *      1   +   +*  +
 *
 *      2
 *
 */
void Test::testCase5()
{
    QPair<int, int> block1(0, 0), block2(1, 1);
    QCOMPARE(game->test(map, block1, block2), false);
}
QTEST_MAIN(Test)
