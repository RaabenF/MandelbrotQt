#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)    //formdatei laden
{
    ui->setupUi(this);      //setup form

    setScaleSpinner();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAstroid_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Astroid);
    setScaleSpinner();
}

void MainWindow::on_btnCycloid_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Cycloid);
    setScaleSpinner();
}

void MainWindow::on_btnHuygens_clicked()
{
    this->ui->renderArea->setShape(RenderArea::HuygensCycloid);
    setScaleSpinner();
}

void MainWindow::on_btnHypo_clicked()
{
    this->ui->renderArea->setShape(RenderArea::HypoCycloid);
    setScaleSpinner();
}

void MainWindow::on_btnLine_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Line);
    setScaleSpinner();
}

void MainWindow::on_option1_clicked(bool checked)
{
    this->ui->renderArea->setCool(checked);
}


void MainWindow::on_spinScale_valueChanged(double scale)
{
    this->ui->renderArea->setScale(scale);
}

void MainWindow::setScaleSpinner(){
    this->ui->spinScale->setValue(this->ui->renderArea->scale() );
}





