#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "ui_untitled.h"

class MainWindow : public QMainWindow,
                   public Ui_MainWindow
{
    Q_OBJECT
private:

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void savefile();
    void close();
};

#endif // MAINWINDOW_H
