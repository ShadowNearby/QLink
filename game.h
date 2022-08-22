#pragma once
#include "block.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QLCDNumber>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QTimerEvent>
#include <QVector>
const int LENGTH = 12;
const int HEIGHT = 8;
const int SETTED_TIME = 30;
const QString pattern[5] = { "0", "1", "2", "3", "4" };
const QString tools[4] = { "Shuffle", "Hint", "+1s", "Flash" };
class Game : public QWidget {
    friend class Player;
    Q_OBJECT
public:
    explicit Game(int mode = 0, QWidget* parent = nullptr);
public slots:
    void updateTime();
    void hintEnd();

protected:
    void keyPressEvent(QKeyEvent*);

private:
    Block* block_[LENGTH][HEIGHT];
    Block *player_, *player2_;
    Block *selectedBlock_, *selectedBlock2_;
    Block* tool_;
    QTimer *timer_, *hintTimer_;
    int time_, hintStopTime_;
    QLCDNumber* displayTime_;
    Block *hintBlock1_, *hintBlock2_;
    QLabel* text;
    bool hinting_;
    bool isPause_;
    bool doubleMode_;
    void moveLeft(Block*);
    void moveRight(Block*);
    void moveUp(Block*);
    void moveDown(Block*);
    void createBlocks();
    void createAnotherBlock(int index);
    void createTool();
    void createPlayer();
    bool check(Block*, Block*);
    void erase(Block*, Block*);
    bool verticalCheck(int, int, int, int);
    bool horizontalCheck(int, int, int, int);
    bool oneTurnCheck(int, int, int, int);
    bool twoTurnCheck(int, int, int, int);
    void isOver();
    void shuffle();
    void swapBlock(int, int, int, int);
    void hint();
    void pause();
    void flash();
    void plusSecond();
    void autoFind();
    void clear();
    void setTime();
    void toolActive();
    void save();
    void load();
};
