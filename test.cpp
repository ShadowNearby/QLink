#include "test.h"
#include "game.h"
Test::Test()
{
}
void Test::case1()
{
    QVERIFY(1 + 1 == 2);
}
void Test::case2()
{
    QVERIFY(1 + 1 == 3);
}
QTEST_MAIN(Test)
