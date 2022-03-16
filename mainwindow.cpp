#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)    //formdatei laden
{
    ui->setupUi(this);      //setup form
}

MainWindow::~MainWindow()
{
    delete ui;
}

