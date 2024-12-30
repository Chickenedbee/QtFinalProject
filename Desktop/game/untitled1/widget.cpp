#include "Widget.h"
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QQueue>
#include <QPoint>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QMainWindow(parent), layout(new QGridLayout), grid(rows, QVector<int>(cols, 0)), flags(rows, QVector<bool>(cols, false)) // flags 用來追蹤每個格子是否放置了旗子
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    for (int i = 0; i < rows; ++i) {
        QVector<QPushButton*> buttonRow;
        for (int j = 0; j < cols; ++j) {
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(30, 30);  // 設定格子大小
            layout->addWidget(button, i, j);
            buttonRow.append(button);

            connect(button, &QPushButton::clicked, this, &Widget::onButtonClicked);
            button->installEventFilter(this);  // 安裝事件過濾器
            
            button->setProperty("row",i);
            button->setProperty("col",j);//設定按鈕座標
        }
        buttons.append(buttonRow);
    }

    initializeGame();  // 初始化遊戲
    mainLayout->addLayout(layout);
    setCentralWidget(centralWidget);
}

Widget::~Widget() {}

bool Widget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::RightButton) {
            QPushButton *button = qobject_cast<QPushButton *>(obj);  // 獲取點擊的按鈕
            if (button) {
                onRightClick(button);  // 處理右鍵事件
                return true;  // 阻止事件繼續傳遞
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);  // 交給父類處理其他事件
}

void Widget::onRightClick(QPushButton *button) {
    if (button->text() == "🚩") {
        button->setText("");  // 移除旗子
        flags[button->property("row").toInt()][button->property("col").toInt()] = false;  // 設置旗子狀態為false
    } else {
        button->setText("🚩");  // 放置旗子
        flags[button->property("row").toInt()][button->property("col").toInt()] = true;  // 設置旗子狀態為true
    }
}

void Widget::onButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (buttons[i][j] == button) {
                if (flags[i][j]) return;  // 如果該格子已經放置了旗子，則不處理點擊
                reveal(i, j);
                return;
            }
        }
    }
}

void Widget::initializeGame() {
    // 隨機放置地雷
    for (int i = 0; i < mineCount;) {
        int r = QRandomGenerator::global()->bounded(rows);
        int c = QRandomGenerator::global()->bounded(cols);
        if (grid[r][c] != -1) { // 避免重複放置地雷
            grid[r][c] = -1;
            ++i;
        }
    }

    // 計算每個格子的周圍地雷數
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == -1) continue;
            grid[i][j] = countMinesAround(i, j);
        }
    }
}

int Widget::countMinesAround(int row, int col) {
    int mineCount = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // 忽略自己
            int newRow = row + i;
            int newCol = col + j;
            if (isValid(newRow, newCol) && grid[newRow][newCol] == -1) {
                ++mineCount;
            }
        }
    }
    return mineCount;
}

void Widget::reveal(int row, int col) {
    if (!isValid(row, col) || buttons[row][col]->isEnabled() == false) return;

    buttons[row][col]->setEnabled(false);

    if (grid[row][col] == -1) { // 點到地雷
        buttons[row][col]->setText("💣");
        revealAllBombs();
        disableAllButtons();
    } else if (grid[row][col] > 0) { // 點到數字
        buttons[row][col]->setText(QString::number(grid[row][col]));
    } else { // 點到空白
        buttons[row][col]->setText("");
        expandEmptyArea(row, col);
    }
}

bool Widget::isValid(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void Widget::expandEmptyArea(int row, int col) {
    QQueue<QPoint> queue;
    QSet<QPoint> visited;  // 用來記錄已經處理的格子
    queue.enqueue(QPoint(row, col));

    while (!queue.isEmpty()) {
        QPoint point = queue.dequeue();
        int r = point.x();
        int c = point.y();

        if (!isValid(r, c) || visited.contains(QPoint(r, c))) continue;

        visited.insert(QPoint(r, c));
        buttons[r][c]->setEnabled(false);
        buttons[r][c]->setText(""); // 顯示空白

        if (grid[r][c] == 0) {
            // 如果該區域是空白，繼續將周圍區域加入隊列
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) continue;
                    queue.enqueue(QPoint(r + i, c + j));
                }
            }
        } else if (grid[r][c] > 0) {
            buttons[r][c]->setText(QString::number(grid[r][c]));
        }
    }
}

void Widget::revealAllBombs() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == -1) {
                buttons[i][j]->setText("💣");
            } else if (grid[i][j] > 0) {
                buttons[i][j]->setText(QString::number(grid[i][j]));
            }
            buttons[i][j]->setEnabled(false);
        }
    }
    QMessageBox *messageBox = new QMessageBox(this);
    messageBox->setWindowTitle("Game Over");
    messageBox->setText("遊戲結束! 再來一場?");
    messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    connect(messageBox, &QMessageBox::buttonClicked, this, [this, messageBox](QAbstractButton *button) {
        if (messageBox->buttonRole(button) == QMessageBox::YesRole) {
            resetGame();  // 重置遊戲
        }
        messageBox->deleteLater();
    });

    messageBox->show();
}

    // 彈出對話框詢問是否重置遊戲
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Game Over", "遊戲結束! 再來一場?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        resetGame();  // 重置遊戲
    }
    // 如果玩家選擇 No，遊戲將保持結束狀態
}


void Widget::disableAllButtons() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            buttons[i][j]->setEnabled(false); // 禁用所有按鈕
        }
    }
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_T) { // 調試模式：顯示所有地雷
        revealAllBombs();
    } else if (event->key() == Qt::Key_R) { // 重置遊戲
        resetGame();
    }
}

void Widget::resetGame() {
    if (grid.size() != rows) grid.resize(rows);
    grid.fill(QVector<int>(cols, 0));

    if (flags.size() != rows) flags.resize(rows);
    flags.fill(QVector<bool>(cols, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            buttons[i][j]->setEnabled(true);
            buttons[i][j]->setText("");
            buttons[i][j]->installEventFilter(this);

            disconnect(buttons[i][j], &QPushButton::clicked, nullptr, nullptr);
            connect(buttons[i][j], &QPushButton::clicked, this, &Widget::onButtonClicked);
        }
    }
    initializeGame();
}

