#pragma once

#include "block.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QLCDNumber>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include <QTimerEvent>
#include <QVector>

const int LENGTH = 16;
const int HEIGHT = 16;
const int NOTE_X = 0;
const int NOTE_Y = 0;
const int SETTED_TIME = 120;
const int TIME_X = 2000;
const int TIME_Y = 200;
const int TIME_LENGTH = 200;
const int TIME_HEIGHT = 100;
const int LINK_LAST_TIME = 1000;
const QString pattern[5] = { "0", "1", "2", "3", "4" };
const QString tools[4] = { "Shuffle", "Hint", "+1s", "Flash" };

class Game : public QWidget {
    friend class Player;

    Q_OBJECT
public:
    explicit Game(int mode = 0, QWidget* parent = nullptr);

private slots:

    void updateTime();

    void hintEnd();

    void save();

    void pause();

    void linkEnd();

private:
    Block* block_[LENGTH][HEIGHT];
    Block *player_, *player2_;
    Block *selectedBlock_, *selectedBlock2_;
    Block* tool_;
    QTimer *timer_, *hintTimer_, *linkTimer_;
    QLCDNumber *displayTime_, *displayTime2_;
    QLCDNumber *displayMark_, *displayMark2_;
    Block *hintBlock1_, *hintBlock2_;
    QPushButton *btn1, *btn2, *btn3;
    QVector<QPair<QPoint, QPoint>> linkPoint_;
    int time_, hintStopTime_;
    int mark_, mark2_;
    bool hinting_;
    bool isPause_;
    bool doubleMode_;

    void moveLeft(Block*);

    void moveRight(Block*);

    void moveUp(Block*);

    void moveDown(Block*);

    void createBlocks();

    void createAnotherBlock(int);

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

    void appendLinePoint(int, int, int, int);

    void setMark();

    void markUp(Block*);

    void hint();

    void flash();

    void plusSecond();

    void autoFind();

    void clear();

    void setTime();

    void toolActive();

    void load();

    void setButton();

    void keyPressEvent(QKeyEvent*);

    void paintEvent(QPaintEvent*);
};
