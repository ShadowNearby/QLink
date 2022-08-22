#include "widget.h"
#include "ui_widget.h"
#include <QTextEdit>
Widget::Widget(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    //    this->setStyleSheet("border-image:url(:/img/main.png);");
    QPalette pal;
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/img/home.png")));
    this->setPalette(pal);
    this->setFixedSize(900, 900);
    this->show();
    buttonSet();
}

Widget::~Widget()
{
}
void Widget::buttonSet()
{

    btn1 = new QPushButton("单人模式");
    btn1->setFixedSize(100, 50);
    btn1->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    btn1->setStyleSheet("QPushButton{"
                        "border:2px rgb(237, 241, 255);"
                        "border-radius:50px;"
                        "padding:2px 3px;"
                        "border-style: outset;"
                        "};"
                        "QPushButton:hover{"
                        "color: black;"
                        "}"
                        "QPushButton:pressed{"
                        ""
                        "border-style: inset;"
                        "}");

    btn1->move(1230, 600);
    btn1->show();
    connect(btn1, SIGNAL(clicked()), this, SLOT(singleMode()));
    btn2 = new QPushButton("读取存档");
    btn2->setFixedSize(100, 50);
    btn2->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    btn2->move(1230, 700);
    btn2->setStyleSheet("QPushButton{"
                        "border:2px rgb(237, 241, 255);"
                        "border-radius:50px;"
                        "padding:2px 3px;"
                        "border-style: outset;"
                        "};"
                        "QPushButton:hover{"
                        "color: black;"
                        "}"
                        "QPushButton:pressed{"
                        ""
                        "border-style: inset;"
                        "}");
    btn2->show();
    connect(btn2, SIGNAL(clicked()), this, SLOT(loadMode()));
    btn3 = new QPushButton("双人模式");
    btn3->setFixedSize(100, 50);
    btn3->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    btn3->move(1230, 800);
    btn3->setStyleSheet("QPushButton{"
                        "border:2px rgb(237, 241, 255);"
                        "border-radius:50px;"
                        "padding:2px 3px;"
                        "border-style: outset;"
                        "};"
                        "QPushButton:hover{"
                        "color: black;"
                        "}"
                        "QPushButton:pressed{"
                        ""
                        "border-style: inset;"
                        "}");
    btn3->show();
    connect(btn3, SIGNAL(clicked()), this, SLOT(doubleMode()));
    btn4 = new QPushButton("退出");
    btn4->setFixedSize(100, 50);
    btn4->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    btn4->move(1230, 900);
    btn4->setStyleSheet("QPushButton{"
                        "border:2px rgb(237, 241, 255);"
                        "border-radius:50px;"
                        "padding:2px 3px;"
                        "border-style: outset;"
                        "};"
                        "QPushButton:hover{"
                        "color: black;"
                        "}"
                        "QPushButton:pressed{"
                        ""
                        "border-style: inset;"
                        "}");
    btn4->show();
    connect(btn4, SIGNAL(clicked()), this, SLOT(quit()));
}
void Widget::singleMode()
{
    game = new Game(0);
    game->show();
    btn1->close();
    btn2->close();
    btn3->close();
    btn4->close();
    this->close();
}
void Widget::loadMode()
{
    game = new Game(1);
    game->show();
    btn1->close();
    btn2->close();
    btn3->close();
    btn4->close();
    this->close();
}
void Widget::doubleMode()
{
    game = new Game(2);
    game->show();
    btn1->close();
    btn2->close();
    btn3->close();
    btn4->close();
    this->close();
}
void Widget::quit()
{
    this->close();
}
