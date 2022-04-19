#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::MainWindow)    //load form, with arg: parent
    //RenderArea is implemented through UI
    ,modelShape(new QStringListModel(this) )
{
    ui->setupUi(this);      //setup form
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //this->move(-900,200);           //place window
    //this->ui->renderArea->setShape(0);
    this->ui->lcdZoom->setSegmentStyle(QLCDNumber::Filled);
    //todo set background color
    //this->ui->lcdZoom->setBackgroundRole()

    // Populate our model of Shaplist for the scrollable Menulist
    modelShape->setStringList(ui->renderArea->ShapeList);    //string-list-model mShapeList
    ui->lvShape->setModel(modelShape);    // Glue model and (List)view together

    //debugging stuff:
    QScreen *screen = QGuiApplication::screens()[0];    //doesnt matter wich screen, with virtual desktop
    this->move(screen->availableGeometry().width() - this->width(), screen->availableGeometry().height() -this->height() );
    //QScreen *screen = QGuiApplication::screens()[QGuiApplication::screens().size()-1];
    //this->setScreen(screen );     //see doc, doesnt move automatic, not on virtual screens
    if(this->hasMouseTracking() ){
        this->setMouseTracking(true);
        qDebug("mousetracking on");
    }else qDebug("mousetracking not available\n");

    connect(ui->renderArea, SIGNAL(valueChanged() ), this, SLOT(on_ui_val_changed()) );
    this->ui->renderArea->setShape(this->ui->renderArea->getShapeIDbyName("mandel brot") );
    update_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setStepSpinnerStep(unsigned int steplength){
    ui->spinCount->setSingleStep(steplength);
}

void MainWindow::update_ui(){
    this->ui->spinCount->setValue(this->ui->renderArea->stepCount() );

    this->ui->lcdZoom->display(this->ui->renderArea->scale() );         //scale=zoom
    if(this->ui->renderArea->stepCount() > this->ui->slideScale->maximum() ){
        this->ui->lcdZoom->setSegmentStyle(QLCDNumber::Outline);
    }else{
        this->ui->lcdZoom->setSegmentStyle(QLCDNumber::Filled);
    }
    this->ui->slideScale->setValue(this->ui->renderArea->scale() );

    this->ui->lcdInterval->display(this->ui->renderArea->Interval() );
    this->ui->slideInterval->setValue(10 * this->ui->renderArea->Interval() );      //int max 200/10    int slider zu float konvert

    this->ui->spinCount->setValue(this->ui->renderArea->stepCount() );
}


void MainWindow::on_spinCount_valueChanged(double spinValue)
{
    this->ui->renderArea->setStepCount(spinValue);
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


void MainWindow::on_slideScale_valueChanged(int value)  //Zoom slider
{
    this->ui->renderArea->setScale(value);
    update_ui();
}
void MainWindow::on_slideInterval_valueChanged(int value)
{
    //int-slider changes float value while casting. -> Random behaviour by QT
    float tInterval = this->ui->renderArea->Interval() - value/10;
    if( tInterval >=1 || tInterval <= -1 ){
        this->ui->renderArea->setInterval(value/10);    //slider int->float auto cast
        update_ui();
    }
}


void MainWindow::on_lvShape_clicked(const QModelIndex &index)   //listview clicked
{
    //QString itemText = index.data(Qt::DisplayRole).toString();    //Text des List Items
    ui->renderArea->setShape(index.row() );
    //set resolution of the step spinner
    if(ui->renderArea->getActShapeID() >= ui->renderArea->getShapeIDbyName("mandel brot") ) setStepSpinnerStep(1);
    else setStepSpinnerStep(32);

    update_ui();
}

void MainWindow::on_ui_val_changed(){
    update_ui();
};

//https://doc.qt.io/qt-6/qlcdnumber.html#segmentStyle-prop
//QLCDNumber::Outline	0	gives raised segments filled with the background color.
//QLCDNumber::Filled	1	gives raised segments filled with the windowText color.
//QLCDNumber::Flat	2	gives flat segments filled with the windowText color.

