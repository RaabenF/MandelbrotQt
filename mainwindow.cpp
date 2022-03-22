#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)    //formdatei laden
{
    ui->setupUi(this);      //setup form

    update_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_ui(){
    this->ui->slideScale->setValue(this->ui->renderArea->scale() );
    this->ui->slideInterval->setValue(this->ui->renderArea->Interval() );
    this->ui->spinCount->setValue(this->ui->renderArea->stepCount() );

    this->ui->lcdZoom->display(this->ui->renderArea->scale() );
    this->ui->lcdInterval->display(this->ui->renderArea->Interval() );

}


void MainWindow::on_btnAstroid_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Astroid);
    update_ui();
}

void MainWindow::on_btnCycloid_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Cycloid);
    update_ui();
}

void MainWindow::on_btnHuygens_clicked()
{
    this->ui->renderArea->setShape(RenderArea::HuygensCycloid);
    update_ui();
}

void MainWindow::on_btnHypo_clicked()
{
    this->ui->renderArea->setShape(RenderArea::HypoCycloid);
    update_ui();
}

void MainWindow::on_btnLine_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Line);
    update_ui();
}

void MainWindow::on_option1_clicked(bool checked)
{
    this->ui->renderArea->setCool(checked);
}

void MainWindow::on_slideScale_valueChanged(int value)
{
    this->ui->renderArea->setScale(value);
    update_ui();
}

void MainWindow::on_slideInterval_valueChanged(int value)
{
    this->ui->renderArea->setInterval(value);
    update_ui();
}

void MainWindow::on_spinCount_valueChanged(double arg1)
{
    this->ui->renderArea->setStepCount(arg1);
}

