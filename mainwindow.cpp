#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)    //formdatei laden
    ,modelShape(new QStringListModel(this) )
{
    ui->setupUi(this);      //setup form

    // Populate our model
    modelShape->setStringList(ui->renderArea->ShapeList);    //string-list-model mShapeList
    ui->lvShape->setModel(modelShape);    // Glue model and view together

    update_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_ui(){
    this->ui->spinCount->setValue(this->ui->renderArea->stepCount() );

    this->ui->lcdZoom->display(this->ui->renderArea->scale() );         //scale=zoom=stepCount
    this->ui->slideScale->setValue(this->ui->renderArea->scale() );

    this->ui->lcdInterval->display(this->ui->renderArea->Interval() );
    this->ui->slideInterval->setValue(10 * this->ui->renderArea->Interval() );      //int max 200/10    int slider zu float konvert
}


void MainWindow::on_spinCount_valueChanged(double arg1)
{
    this->ui->renderArea->setStepCount(arg1);
}
void MainWindow::on_option1_clicked(bool checked)
{
    this->ui->renderArea->setCool(checked);
}

void MainWindow::on_btnBackground_clicked()
{
    QColor color = QColorDialog::getColor(ui->renderArea->backgroundColor(), this, "Select Color");
    ui->renderArea->setBackgroundColor(color);
}
void MainWindow::on_btnLineColor_clicked()
{
    QColor color = QColorDialog::getColor(ui->renderArea->ShapeColor(), this, "Select Color");
    ui->renderArea->setShapeColor(color);
}


void MainWindow::on_slideScale_valueChanged(int value)
{
    this->ui->renderArea->setScale(value);
    update_ui();
}
void MainWindow::on_slideInterval_valueChanged(int value)
{
    //int-slider changes float value while casting
    float temp = value/10;
    float tempinterval = this->ui->renderArea->Interval();
            tempinterval = temp - tempinterval;
    if(0.5< tempinterval || -0.5> tempinterval ){
        if(!prevent_slideCast){
            this->ui->renderArea->setInterval(temp);    //float->int
            update_ui();
            prevent_slideCast = true;
        }
        else prevent_slideCast = false;
    }
}


void MainWindow::on_lvShape_clicked(const QModelIndex &index)
{
    //QString itemText = index.data(Qt::DisplayRole).toString();    //Text des List Items
    ui->renderArea->setShape(index.row() );
    update_ui();
}

