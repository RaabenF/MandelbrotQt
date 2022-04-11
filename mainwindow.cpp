#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)    //formdatei laden
    ,modelShape(new QStringListModel(this) )
{
    ui.setupUi(this);      //setup form
    //this->setAttribute(Qt::WA_TranslucentBackground);

    // Populate our model of Shaplist for the scrollable Menulist
    modelShape->setStringList(ui.renderArea->ShapeList);    //string-list-model mShapeList
    ui.lvShape->setModel(modelShape);    // Glue model and (List)view together

    //this->geometry()
    //this->setScreen()
    this->ui.renderArea->setShape(0);
    update_ui();
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::setStepSpinStep(unsigned int steplength){
    ui.spinCount->setSingleStep(steplength);
}

void MainWindow::update_ui(){
    this->ui.spinCount->setValue(this->ui.renderArea->stepCount() );

    this->ui.lcdZoom->display(this->ui.renderArea->scale() );         //scale=zoom
    this->ui.slideScale->setValue(this->ui.renderArea->scale() );

    this->ui.lcdInterval->display(this->ui.renderArea->Interval() );
    this->ui.slideInterval->setValue(10 * this->ui.renderArea->Interval() );      //int max 200/10    int slider zu float konvert
}


void MainWindow::on_spinCount_valueChanged(double arg1)
{
    this->ui.renderArea->setStepCount(arg1);
}
void MainWindow::on_option1_clicked(bool checked)
{
    this->ui.renderArea->setCool(checked);
}

void MainWindow::on_btnBackground_clicked()
{
    QColor color = QColorDialog::getColor(ui.renderArea->backgroundColor(), this, "Select Color");
    ui.renderArea->setBackgroundColor(color);
}
void MainWindow::on_btnLineColor_clicked()
{
    QColor color = QColorDialog::getColor(ui.renderArea->ShapeColor(), this, "Select Color");
    ui.renderArea->setShapeColor(color);
}


void MainWindow::on_slideScale_valueChanged(int value)
{
    this->ui.renderArea->setScale(value);
    update_ui();
}
void MainWindow::on_slideInterval_valueChanged(int value)
{
    //int-slider changes float value while casting. -> Random behaviour by QT
    float tInterval = this->ui.renderArea->Interval() - value/10;
    if( tInterval >=1 || tInterval <= -1 ){
        this->ui.renderArea->setInterval(value/10);    //slider int->float auto cast
        update_ui();
    }
}


void MainWindow::on_lvShape_clicked(const QModelIndex &index)   //listview clicked
{
    //QString itemText = index.data(Qt::DisplayRole).toString();    //Text des List Items
    ui.renderArea->setShape(index.row() );
    if(ui.renderArea->getActShapeID() == ui.renderArea->getShapeIDbyName("mandel brot") ) setStepSpinStep(1);
    else setStepSpinStep(8);
    update_ui();
}

