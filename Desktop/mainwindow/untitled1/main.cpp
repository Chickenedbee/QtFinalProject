#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 建立主視窗
    MainWindow mainWindow;

    // 顯示主視窗
    mainWindow.show();

    // 執行應用程式事件迴圈
    return app.exec();
}
