#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QVector>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QQueue>
#include <QPoint>
#include <QSize>
#include <QSet>
#include <QSoundEffect>
class Widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    int rows = 10;          // 行數
    int cols = 10;          // 列數
    int mineCount = 10;     // 地雷數量
    int flagCount = 0;
    int cerrectCount = 0;

    QLineEdit *rowsInput;
    QLineEdit *colsInput;
    QLineEdit *mineCountInput;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    QGridLayout *layout;          // 網格佈局
    QVector<QVector<int>> grid;  // 儲存遊戲格子狀態，-1 代表地雷
    QVector<QVector<QPushButton*>> buttons;  // 儲存所有按鈕
    QVector<QVector<bool>> flags; // 儲存格子是否放置旗子


    void initializeGame();  // 初始化遊戲

    void theDifficultyWidget(); // 選擇難度介面
    void setEasy();
    void setNormal();
    void setHard();
    void setCustomise();

    void resetGrid(); // 重置陣列
    void setButton(); // 設定操控按鈕

    void reveal(int row, int col);  // 顯示格子的內容
    bool isValid(int row, int col);  // 檢查格子是否有效
    int countMinesAround(int row, int col);  // 計算周圍地雷數量
    void expandEmptyArea(int row, int col);  // 展開空白區域
    void revealAllBombs();  // 顯示所有地雷
    void disableAllButtons();  // 禁用所有按鈕
    void resetGame();  // 重置遊戲
    void onRightClick(QPushButton *button);  // 右鍵點擊事件處理
    void onButtonClicked();  // 按鈕點擊事件處理

    QSoundEffect clickSound;
    QSoundEffect flagSound;
    QSoundEffect mineSound;
    QSoundEffect winSound;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;  // 事件過濾器
    void keyPressEvent(QKeyEvent *event) override;  // 鍵盤事件

};

#endif // WIDGET_H
