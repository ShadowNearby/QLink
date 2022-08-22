#pragma once
#include <QDebug>
#include <QPushButton>
const int BLOCK_SIZE = 65;
const int MARGIN = 1;
const int OFFSET_X = 850;
const int OFFSET_Y = 450;
class Block : public QPushButton {

    Q_OBJECT

public:
    explicit Block(QWidget* parent = nullptr);
    explicit Block(const QString& text, QWidget* parent = nullptr);
    Block(int, QWidget* parent = nullptr);
    void setIndex(int);
    int getIndex() const;
    void setIsSelected(bool isSelected = false);
    bool getIsSelected() const;
    bool getIsTool() const;
    void setIsTool(bool isTool = false);
    int getX();
    int getY();

private:
    int index_;
    bool isSelected_;
    bool isTool_;
};
