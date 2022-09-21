#pragma once

#include <QtTest/QtTest>

class Test : public QObject {
    Q_OBJECT
private slots:
    void case1();

    void case2();

public:
    Test();
};
