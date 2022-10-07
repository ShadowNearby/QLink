#pragma once

#include "game.h"
#include <QtTest/QtTest>
class Test : public QObject {
    Q_OBJECT
public:
    Test();
private slots:
    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();
    void testCase5();

private:
    Game* game;
};
