#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <qtreeview.h>
#include <QStringListModel>

#include <ui_mainwindow.h>  //versucht ohne namespace Ui zu machen, geht

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setStepSpinStep(unsigned int steplength);

private slots:

    void on_option1_clicked(bool checked);

    void on_slideInterval_valueChanged(int value);

    void on_slideScale_valueChanged(int value);

    void on_spinCount_valueChanged(double arg1);

    void on_btnBackground_clicked();

    void on_btnLineColor_clicked();

    void on_lvShape_clicked(const QModelIndex &index);


private:
    //Ui::MainWindow *ui;   //ist gleich:
    Ui_MainWindow ui;
    QStringListModel *modelShape;
    float lastInterval;

    void update_ui();

};
#endif // MAINWINDOW_H
