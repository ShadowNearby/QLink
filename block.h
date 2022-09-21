#pragma once

#include <QDebug>
#include <QPushButton>

const int BLOCK_SIZE = 65;
const int MARGIN = 1;
const int OFFSET_X = 850;
const int OFFSET_Y = 450;
enum BlockIndex {
    block_yellow,
    block_green,
    block_blue,
    block_red,
    block_purple,
    block_brown,
    shuffle,
    hint,
    plus,
    player,
    player2
};

class Block : public QPushButton {

    Q_OBJECT

public:
    explicit Block(QWidget* parent = nullptr);

    explicit Block(const QString& text, QWidget* parent = nullptr);

    explicit Block(BlockIndex, QWidget* parent = nullptr);

    void setIndex(int);

    int getIndex() const;

    void setIsSelected(bool isSelected = false);

    bool getIsSelected() const;

    bool getIsTool() const;

    void setIsTool(bool isTool = false);

    int getX() const;

    int getY() const;

private:
    int index_;
    bool isSelected_;
    bool isTool_;
};
