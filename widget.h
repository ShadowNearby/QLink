#pragma once

#include "game.h"
#include <QWidget>

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();
public slots:
    void singleMode();
    void loadMode();
    void doubleMode();
    void quit();

private:
    void buttonSet();
    Game* game;
    QPushButton *btn1, *btn2, *btn3, *btn4, *btn5;
};
