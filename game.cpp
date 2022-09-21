#include "game.h"
//
Game::Game(int mode, QWidget* parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    QPalette pal;
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/img/bg_4x.png")));
    this->setPalette(pal);
    this->showFullScreen();
    this->show();
    srand(time(NULL));
    if (mode == 0) {
        doubleMode_ = false;
        clear();
        createBlocks();
        createPlayer();
        createTool();
        time_ = SETTED_TIME;
    }
    if (mode == 1) {
        load();
    }
    if (mode == 2) {
        doubleMode_ = true;
        clear();
        createBlocks();
        createPlayer();
        createTool();
        time_ = SETTED_TIME;
    }
    setTime();
    setMark();
    setButton();
    isPause_ = false;
}

void Game::setButton()
{
    btn1 = new QPushButton(this);
    btn1 = new QPushButton("保存并退出 /q");
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

    btn1->move(100, 800);
    btn1->show();
    connect(btn1, SIGNAL(clicked()), this, SLOT(save()));
    btn2 = new QPushButton(this);
    btn2 = new QPushButton("暂停 /p");
    btn2->setFixedSize(100, 50);
    btn2->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
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

    btn2->move(100, 600);
    btn2->show();
    connect(btn2, SIGNAL(clicked()), this, SLOT(pause()));
}

void Game::load()
{
    //  打开存档的json文件
    clear();
    QFile saveFile("./save.json");
    saveFile.open(QIODevice::ReadOnly);
    // 读取存档文件
    QJsonDocument document(QJsonDocument::fromJson(saveFile.readAll()));
    saveFile.close();
    QJsonObject gameObject = document.object();
    player_ = new Block(BlockIndex(player));
    int player_x = gameObject["player_x"].toInt(),
        player_y = gameObject["player_y"].toInt();
    player_->move(player_x, player_y);
    mark_ = gameObject["mark"].toInt();
    bool mode = gameObject["double_mode"].toBool();
    doubleMode_ = false;
    if (mode) {
        // 设置为双人模式
        doubleMode_ = true;
        player2_ = new Block(BlockIndex(player2));
        int player2_x = gameObject["player2_x"].toInt(),
            player2_y = gameObject["player2_y"].toInt();
        player2_->move(player2_x, player2_y);
        mark2_ = gameObject["mark2"].toInt();
    }
    time_ = gameObject["time"].toInt();
    if (!gameObject["tool_index"].isNull()) {
        // 确定工具种类
        tool_ = new Block(BlockIndex(gameObject["tool_index"].toInt()));
        tool_->setIndex(gameObject["tool_index"].toInt());
        // 定位工具位置
        block_[(gameObject["tool_x"].toInt() - OFFSET_X) / BLOCK_SIZE][(gameObject["tool_y"].toInt() - OFFSET_Y) / BLOCK_SIZE] = tool_;
        tool_->move(gameObject["tool_x"].toInt(), gameObject["tool_y"].toInt());
    }
    QJsonArray blockArray = gameObject["blocks"].toArray();
    for (int blockIndex = 0; blockIndex < blockArray.size(); ++blockIndex) {
        int block_x = blockArray.at(blockIndex)["block_x"].toInt(),
            block_y = blockArray.at(blockIndex)["block_y"].toInt(),
            block_index = blockArray.at(blockIndex)["block_index"].toInt();
        block_[block_x][block_y] = new Block(BlockIndex(block_index));
        block_[block_x][block_y]->setIndex(block_index);
        block_[block_x][block_y]->move(block_x * BLOCK_SIZE + OFFSET_X, block_y * BLOCK_SIZE + OFFSET_Y);
        block_[block_x][block_y]->setIsTool(false);
    }
}

void Game::save()
{
    QFile saveFile("./save.json");
    saveFile.open(QIODevice::WriteOnly);
    QJsonObject gameObject;
    gameObject.insert("player_x", player_->x());
    gameObject.insert("player_y", player_->y());
    gameObject.insert("time", time_);
    gameObject.insert("double_mode", this->doubleMode_);
    gameObject.insert("mark", mark_);
    if (doubleMode_) {
        gameObject.insert("player2_x", player2_->x());
        gameObject.insert("player2_y", player2_->y());
        gameObject.insert("mark2", mark2_);
    }
    if (tool_) {
        gameObject.insert("tool_index", tool_->getIndex());
        gameObject.insert("tool_x", tool_->x());
        gameObject.insert("tool_y", tool_->y());
    }
    QJsonArray blockArray;
    for (int i = 1; i < LENGTH - 1; ++i) {
        for (int j = 1; j < HEIGHT - 1; ++j) {
            if (block_[i][j]) {
                if (!block_[i][j]->getIsTool()) {
                    QJsonObject* blockObject = new QJsonObject();
                    blockObject->insert("block_index", block_[i][j]->getIndex());
                    blockObject->insert("block_x", block_[i][j]->getX());
                    blockObject->insert("block_y", block_[i][j]->getY());
                    blockArray.append(*blockObject);
                }
            }
        }
    }
    gameObject.insert("blocks", blockArray);
    QJsonDocument document;
    document.setObject(gameObject);
    saveFile.write(document.toJson());
    saveFile.close();
    this->close();
}

void Game::pause()
{
    isPause_ = !isPause_;
    if (isPause_) {
        timer_->stop();
    } else {
        timer_->start(1000);
    }
}

void Game::autoFind()
{
    QVector<Block*> temp;
    QVector<QPair<QPoint, QPoint>> tempPair = linkPoint_;
    for (int i = 1; i < LENGTH - 1; ++i) {
        for (int j = 1; j < HEIGHT - 1; ++j) {
            if (block_[i - 1][j] && block_[i + 1][j] && block_[i][j - 1] && block_[i][j + 1]) {
                continue;
            }
            temp.append(block_[i][j]);
        }
    }
    for (int i = 0; i < temp.size(); ++i) {
        for (int j = i + 1; j < temp.size(); ++j) {
            if (check(temp.at(i), temp.at(j))) {
                hintBlock1_ = temp.at(i);
                hintBlock2_ = temp.at(j);
                linkPoint_ = tempPair;
                return;
            }
            linkPoint_ = tempPair;
        }
    }
    hintBlock1_ = hintBlock2_ = nullptr;
}

void Game::setTime()
{

    displayTime_ = new QLCDNumber(this);
    displayTime_->setDigitCount(3);
    displayTime_->move(TIME_X, TIME_Y);
    displayTime_->display(time_);
    displayTime_->setFixedSize(TIME_LENGTH, TIME_HEIGHT);
    displayTime_->show();
    timer_ = new QTimer(this);
    hintTimer_ = new QTimer(this);
    linkTimer_ = new QTimer(this);
    connect(hintTimer_, SIGNAL(timeout()), this, SLOT(hintEnd()));
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(linkTimer_, SIGNAL(timeout()), this, SLOT(linkEnd()));
    timer_->start(1000);
    hintStopTime_ = 0;
}

void Game::clear()
{
    for (int i = 0; i < LENGTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            block_[i][j] = nullptr;
        }
    }
    tool_ = nullptr;
    mark_ = mark2_ = 0;
    player_ = player2_ = nullptr;
    selectedBlock_ = selectedBlock2_ = nullptr;
    timer_ = hintTimer_ = nullptr;
    displayMark_ = displayMark2_ = displayTime_ = displayTime2_ = nullptr;
    btn1 = btn2 = btn3 = nullptr;
    hinting_ = false;
}

void Game::createTool()
{
    if (tool_ || hinting_) {
        return;
    }
    hintBlock1_ = hintBlock2_ = nullptr;
    hinting_ = false;
    int index = rand() % 3 + 6;
    tool_ = new Block(BlockIndex(index));
    int i = 0, j = 0;
    while (true) {
        i = rand() % LENGTH;
        j = rand() % HEIGHT;
        if (block_[i][j]) {
            continue;
        }
        if (i == player_->getX() && j == player_->getY()) {
            continue;
        }
        if (doubleMode_ && i == player2_->getX() && j == player2_->getY()) {
            continue;
        }
        break;
    }
    tool_->move(i * BLOCK_SIZE + OFFSET_X, j * BLOCK_SIZE + OFFSET_Y);
    tool_->setIsTool(true);
    tool_->setIndex(index - 6);
    block_[i][j] = tool_;
}

void Game::createPlayer()
{
    player_ = new Block(BlockIndex(player));
    player_->move(OFFSET_X, OFFSET_Y);
    player2_ = nullptr;
    if (doubleMode_) {
        player2_ = new Block(BlockIndex(player2));
        player2_->move(OFFSET_X + (LENGTH - 1) * BLOCK_SIZE, OFFSET_Y + (HEIGHT - 1) * BLOCK_SIZE);
    }
}

void Game::plusSecond()
{
    time_ += SETTED_TIME + 1;
    updateTime();
}

void Game::hint()
{
    hintBlock1_->setEnabled(false);
    hintBlock2_->setEnabled(false);
}

void Game::hintEnd()
{
    hintTimer_->stop();
    hinting_ = false;
    hintBlock1_->setEnabled(true);
    hintBlock2_->setEnabled(true);
}

void Game::flash()
{
}

void Game::shuffle()
{
    int randomX, randomY;
    for (int i = 1; i < LENGTH - 1; ++i) {
        for (int j = 1; j < HEIGHT - 1; ++j) {
            if (this->block_[i][j]) {
                randomX = rand() % (LENGTH - 2) + 1;
                randomY = rand() % (HEIGHT - 2) + 1;
                while (!this->block_[randomX][randomY] || (randomX == i && randomY == j)) {
                    randomX = rand() % (LENGTH - 2) + 1;
                    randomY = rand() % (HEIGHT - 2) + 1;
                }
                swapBlock(i, j, randomX, randomY);
            }
        }
    }
}

void Game::swapBlock(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2) {
        return;
    }
    if (!this->block_[x1][y1] || !this->block_[x2][y2]) {
        return;
    }
    int block1X = this->block_[x1][y1]->x(), block1Y = this->block_[x1][y1]->y(), block2X = this->block_[x2][y2]->x(), block2Y = this->block_[x2][y2]->y();
    this->block_[x1][y1]->move(block2X, block2Y);
    this->block_[x2][y2]->move(block1X, block1Y);
    Block* temp_block = this->block_[x1][y1];
    this->block_[x1][y1] = this->block_[x2][y2];
    this->block_[x2][y2] = temp_block;
}

void Game::appendLinePoint(int x1, int y1, int x2, int y2)
{
    auto toX = [](int x) {
        return (x + 0.5) * 65 + OFFSET_X;
    };
    auto toY = [](int y) {
        return (y + 0.5) * 65 + OFFSET_Y;
    };
    QPair<QPoint, QPoint> pair(QPoint(toX(x1), toY(y1)), QPoint(toX(x2), toY(y2)));
    linkPoint_.append(pair);
}

void Game::linkEnd()
{
    linkPoint_.clear();
    update();
    linkTimer_->stop();
}

void Game::createBlocks()
{
    for (int i = 1; i < LENGTH - 1; ++i) {
        for (int j = 1; j < HEIGHT - 1; ++j) {
            if (block_[i][j]) {
                continue;
            }
            int randomIndex = rand() % 6;
            block_[i][j] = new Block(BlockIndex(randomIndex));
            block_[i][j]->setIndex(randomIndex);
            //            connect(block_[i][j], SIGNAL(blockClicked(int)), this, SLOT(blockClicked(int)));
            block_[i][j]->move(i * BLOCK_SIZE + OFFSET_X, j * BLOCK_SIZE + OFFSET_Y);
            //            block_[i][j]->setEnabled(false);
            createAnotherBlock(randomIndex);
        }
    }
}

void Game::createAnotherBlock(int index)
{
    int i = rand() % (LENGTH - 2) + 1, j = rand() % (HEIGHT - 2) + 1;
    while (block_[i][j]) {
        i = rand() % (LENGTH - 2) + 1;
        j = rand() % (HEIGHT - 2) + 1;
    }
    block_[i][j] = new Block(BlockIndex(index));
    block_[i][j]->setIndex(index);
    block_[i][j]->move(i * BLOCK_SIZE + OFFSET_X, j * BLOCK_SIZE + OFFSET_Y);
    //    block_[i][j]->setEnabled(false);
}

void Game::updateTime()
{
    --time_;
    displayTime_->display(time_);
    if (time_ == 0) {
        timer_->stop();
        QMessageBox messageBox(QMessageBox::NoIcon,
            "", "",
            QMessageBox::Close, NULL);
        messageBox.setWindowFlags(Qt::FramelessWindowHint);

        QString msg = "游戏结束!";
        if (doubleMode_) {
            if (mark_ > mark2_)
                msg += " p1胜利";
            else if (mark_ < mark2_)
                msg += " p2胜利";
            else
                msg += " 平局";
        }
        messageBox.setText(msg);
        int result = messageBox.exec();
        switch (result) {
        case QMessageBox::Close:
            this->close();
            break;
        default:
            break;
        }
    }
    if (time_ % 20 == 0) {
        createTool();
    }
}

bool Game::horizontalCheck(int x1, int y1, int x2, int y2)
{
    if (y1 != y2) {
        return false;
    }
    int maxX = std::max(x1, x2), minX = std::min(x1, x2);
    if (maxX - minX == 1) {
        appendLinePoint(x1, y1, x2, y2);
        return true;
    }
    for (int i = minX + 1; i < maxX; ++i) {
        if (this->block_[i][y1]) {
            return false;
        }
    }
    appendLinePoint(x1, y1, x2, y2);
    return true;
}

bool Game::verticalCheck(int x1, int y1, int x2, int y2)
{
    if (x1 != x2) {
        return false;
    }
    int maxY = std::max(y1, y2), minY = std::min(y1, y2);
    if (maxY - minY == 1) {
        appendLinePoint(x1, y1, x2, y2);
        return true;
    }
    for (int i = minY + 1; i < maxY; ++i) {
        if (this->block_[x1][i]) {
            return false;
        }
    }
    appendLinePoint(x1, y1, x2, y2);
    return true;
}

bool Game::oneTurnCheck(int x1, int y1, int x2, int y2)
{
    if (x1 != x2 && y1 != y2) {
        if (!this->block_[x1][y2]) {
            if (verticalCheck(x1, y1, x1, y2) && horizontalCheck(x2, y2, x1, y2)) {
                return true;
            }
        }
        if (!this->block_[x2][y1]) {
            if (verticalCheck(x2, y2, x2, y1) && horizontalCheck(x1, y1, x2, y1)) {
                return true;
            }
        }
        return false;
    }
    return false;
}

bool Game::twoTurnCheck(int x1, int y1, int x2, int y2)
{

    for (int i = 0; i < LENGTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            if (this->block_[i][j]) {
                continue;
            }
            if (i != x1 && i != x2 && j != y1 && j != y2) {
                continue;
            }
            if (oneTurnCheck(x1, y1, i, j)) {
                if (horizontalCheck(x2, y2, i, j) || verticalCheck(x2, y2, i, j)) {
                    return true;
                }

                else {
                    linkPoint_.clear();
                }
            }
            if (oneTurnCheck(x2, y2, i, j)) {
                if (horizontalCheck(x1, y1, i, j) || verticalCheck(x1, y1, i, j)) {
                    return true;
                } else {
                    linkPoint_.clear();
                }
            }
        }
    }
    return false;
}

bool Game::check(Block* block1, Block* block2)
{
    if (!block1 || !block2) {
        return false;
    }
    if (block1 == block2) {
        return false;
    }
    if (block1->getIndex() != block2->getIndex()) {
        return false;
    }
    int x1 = block1->getX(),
        y1 = block1->getY(),
        x2 = block2->getX(),
        y2 = block2->getY();
    if (horizontalCheck(x1, y1, x2, y2)) {
        return true;
    }
    if (verticalCheck(x1, y1, x2, y2)) {
        return true;
    }
    if (oneTurnCheck(x1, y1, x2, y2)) {
        return true;
    }
    if (twoTurnCheck(x1, y1, x2, y2)) {
        return true;
    }
    return false;
}

void Game::erase(Block* block1, Block* block2)
{
    int x1 = block1->getX(),
        y1 = block1->getY(),
        x2 = block2->getX(),
        y2 = block2->getY();
    linkPoint_.clear();
    if (doubleMode_ && block2 == selectedBlock2_) {
        if (!check(block1, block2)) {
            selectedBlock2_->setIsSelected(false);
            selectedBlock2_ = nullptr;
            return;
        }
        delete block_[x1][y1];
        delete block_[x2][y2];
        block_[x1][y1] = nullptr;
        block_[x2][y2] = nullptr;
        selectedBlock2_ = nullptr;
        markUp(player2_);
        isOver();
        if (hinting_) {
            hint();
        }
    } else {
        if (!check(block1, block2)) {
            selectedBlock_->setIsSelected(false);
            selectedBlock_ = nullptr;
            return;
        }
        delete block_[x1][y1];
        delete block_[x2][y2];
        block_[x1][y1] = nullptr;
        block_[x2][y2] = nullptr;
        selectedBlock_ = nullptr;
        markUp(player_);
        isOver();
        if (hinting_) {
            hint();
        }
    }
    linkTimer_->start(LINK_LAST_TIME);
    update();
}

void Game::isOver()
{
    autoFind();
    if (!hintBlock1_ || !hintBlock2_) {
        QMessageBox messageBox(QMessageBox::NoIcon,
            "", "",
            QMessageBox::Close, NULL);
        ;
        messageBox.setWindowFlags(Qt::FramelessWindowHint);
        QString msg = "游戏结束!";
        if (doubleMode_) {
            if (mark_ > mark2_)
                msg += " p1胜利";
            else if (mark_ < mark2_)
                msg += " p2胜利";
            else
                msg += " 平局";
        }
        messageBox.setText(msg);
        switch (messageBox.exec()) {
        case QMessageBox::Close:
            this->close();
            break;
        default:
            break;
        }
    }
}

void Game::toolActive()
{
    switch (tool_->getIndex()) {
    case 0:
        shuffle();
        break;
    case 1:
        hintTimer_->start(10000);
        hinting_ = true;
        autoFind();
        hint();
        break;
    case 2:
        plusSecond();
        break;
    case 3:
        flash();
        break;
    default:
        break;
    }
    tool_ = nullptr;
}

void Game::setMark()
{
    displayMark_ = new QLCDNumber(this);
    displayMark_->display(mark_);
    displayMark_->move(100, 100);
    displayMark_->setFixedSize(100, 50);
    displayMark_->show();
    displayMark2_ = nullptr;
    if (doubleMode_) {
        displayMark2_ = new QLCDNumber(this);
        displayMark2_->display(mark_);
        displayMark2_->move(300, 100);
        displayMark2_->setFixedSize(100, 50);
        displayMark2_->show();
    }
}

void Game::markUp(Block* player)
{
    if (player == player_) {
        mark_ += 100;
        displayMark_->display(mark_);
    }
    if (player == player2_) {
        mark2_ += 100;
        displayMark2_->display(mark2_);
    }
}

void Game::moveLeft(Block* player)
{
    int x = player->getX(), y = player->getY();
    if (x == 0) {
        return;
    }
    if (block_[x - 1][y]) {
        if (block_[x - 1][y]->getIsTool()) {
            delete block_[x - 1][y];
            block_[x - 1][y] = nullptr;
            toolActive();
            player->move(player->x() - BLOCK_SIZE, player->y());
            return;
        }
        if (doubleMode_ && player == player2_) {
            if (!selectedBlock2_) {
                block_[x - 1][y]->setIsSelected(true);
                selectedBlock2_ = block_[x - 1][y];
                return;
            }
            erase(block_[x - 1][y], selectedBlock2_);
            return;
        } else {
            if (!selectedBlock_) {
                block_[x - 1][y]->setIsSelected(true);
                selectedBlock_ = block_[x - 1][y];
                return;
            }
            erase(block_[x - 1][y], selectedBlock_);
            return;
        }
    }
    player->move(player->x() - BLOCK_SIZE, player->y());
}

void Game::moveRight(Block* player)
{
    int x = player->getX(), y = player->getY();
    if (x == LENGTH - 1) {
        return;
    }
    if (block_[x + 1][y]) {
        if (block_[x + 1][y]->getIsTool()) {
            delete block_[x + 1][y];
            block_[x + 1][y] = nullptr;
            toolActive();
            player->move(player->x() + BLOCK_SIZE, player->y());
            return;
        }
        if (doubleMode_ && player == player2_) {
            if (!selectedBlock2_) {
                block_[x + 1][y]->setIsSelected(true);
                selectedBlock2_ = block_[x + 1][y];
                return;
            }
            erase(block_[x + 1][y], selectedBlock2_);
            return;
        } else {
            if (!selectedBlock_) {
                block_[x + 1][y]->setIsSelected(true);
                selectedBlock_ = block_[x + 1][y];
                return;
            }
            erase(block_[x + 1][y], selectedBlock_);
            return;
        }
    }
    player->move(player->x() + BLOCK_SIZE, player->y());
}

void Game::moveUp(Block* player)
{
    int x = player->getX(), y = player->getY();
    if (y == 0) {
        return;
    }
    if (block_[x][y - 1]) {
        if (block_[x][y - 1]->getIsTool()) {
            delete block_[x][y - 1];
            block_[x][y - 1] = nullptr;
            toolActive();
            player->move(player->x(), player->y() - BLOCK_SIZE);
            return;
        }
        if (doubleMode_ && player == player2_) {
            if (!selectedBlock2_) {
                block_[x][y - 1]->setIsSelected(true);
                selectedBlock2_ = block_[x][y - 1];
                return;
            }
            erase(block_[x][y - 1], selectedBlock2_);
            return;
        } else {
            if (!selectedBlock_) {
                block_[x][y - 1]->setIsSelected(true);
                selectedBlock_ = block_[x][y - 1];
                return;
            }
            erase(block_[x][y - 1], selectedBlock_);
            return;
        }
    }
    player->move(player->x(), player->y() - BLOCK_SIZE);
}

void Game::moveDown(Block* player)
{
    int x = player->getX(), y = player->getY();
    if (y == HEIGHT - 1) {
        return;
    }
    if (block_[x][y + 1]) {
        if (block_[x][y + 1]->getIsTool()) {
            delete block_[x][y + 1];
            block_[x][y + 1] = nullptr;
            toolActive();
            player->move(player->x(), player->y() + BLOCK_SIZE);
            return;
        }
        if (doubleMode_ && player == player2_) {
            if (!selectedBlock2_) {
                block_[x][y + 1]->setIsSelected(true);
                selectedBlock2_ = block_[x][y + 1];
                return;
            }
            erase(block_[x][y + 1], selectedBlock2_);
            return;
        } else {
            if (!selectedBlock_) {
                block_[x][y + 1]->setIsSelected(true);
                selectedBlock_ = block_[x][y + 1];
                return;
            }
            erase(block_[x][y + 1], selectedBlock_);
            return;
        }
    }
    player->move(player->x(), player->y() + BLOCK_SIZE);
}

void Game::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_P) {
        pause();
        return;
    }
    if (isPause_) {
        return;
    }
    if (!doubleMode_) {
        switch (event->key()) {
        case Qt::Key_A:
            moveLeft(player_);
            break;
        case Qt::Key_D:
            moveRight(player_);
            break;
        case Qt::Key_W:
            moveUp(player_);
            break;
        case Qt::Key_S:
            moveDown(player_);
            break;
        case Qt::Key_Left:
            moveLeft(player_);
            break;
        case Qt::Key_Right:
            moveRight(player_);
            break;
        case Qt::Key_Up:
            moveUp(player_);
            break;
        case Qt::Key_Down:
            moveDown(player_);
            break;
        case Qt::Key_Q:
            save();
            break;
        default:
            break;
        }
    } else {
        switch (event->key()) {
        case Qt::Key_A:
            moveLeft(player_);
            break;
        case Qt::Key_D:
            moveRight(player_);
            break;
        case Qt::Key_W:
            moveUp(player_);
            break;
        case Qt::Key_S:
            moveDown(player_);
            break;
        case Qt::Key_Left:
            moveLeft(player2_);
            break;
        case Qt::Key_Right:
            moveRight(player2_);
            break;
        case Qt::Key_Up:
            moveUp(player2_);
            break;
        case Qt::Key_Down:
            moveDown(player2_);
            break;
        case Qt::Key_Q:
            save();
            break;
        default:
            break;
        }
    }
}

void Game::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    if (linkPoint_.size() > 0) {
        for (int i = 0; i < linkPoint_.size(); ++i) {
            painter.drawLine(linkPoint_.at(i).first, linkPoint_.at(i).second);
        }
    }
}
