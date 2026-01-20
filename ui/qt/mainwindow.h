#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "../../browser/BrowserController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButton();

private:
    QLabel *logoLabel;
    QLineEdit *urlInput;
    QTextEdit *displayArea;
    QPushButton *goButton;
    BrowserController browser;

    void setupUI();
};
