#include "block.h"

Block::Block(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setFixedSize(BLOCK_SIZE - MARGIN, BLOCK_SIZE - MARGIN);
    this->show();
}

Block::Block(BlockIndex pattern, QWidget* parent)
    : QPushButton(parent)
{
    QString path = ":/img/" + QString("%1").arg(pattern) + ".png";
    QIcon icon(path);
    QSize size(BLOCK_SIZE - MARGIN - 1, BLOCK_SIZE - MARGIN - 1);
    this->setIconSize(size);
    this->setIcon(icon);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setStyleSheet("background-color:black;");
    this->setFixedSize(BLOCK_SIZE - MARGIN, BLOCK_SIZE - MARGIN);
    if (pattern <= 5) {
        this->setIsTool(false);
    }

    this->show();
}

void Block::setIndex(int index)
{
    this->index_ = index;
}

int Block::getIndex() const
{
    return index_;
}

void Block::setIsSelected(bool selected)
{
    if (selected)
        this->setStyleSheet("");
    else
        this->setStyleSheet("background-color:black;");
    this->isSelected_ = selected;
}

bool Block::getIsSelected() const
{
    return isSelected_;
}

int Block::getX() const
{
    return (this->x() - OFFSET_X) / BLOCK_SIZE;
}

int Block::getY() const
{
    return (this->y() - OFFSET_Y) / BLOCK_SIZE;
}

void Block::setIsTool(bool isTool)
{
    this->isTool_ = isTool;
}

bool Block::getIsTool() const
{
    return isTool_;
}
