#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <ui_mainwindow.h>  //versucht ohne namespace Ui zu machen, geht

QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAstroid_clicked();

    void on_btnCycloid_clicked();

    void on_btnHuygens_clicked();

    void on_btnHypo_clicked();

private:
    //Ui::MainWindow *ui;   //ist gleich:
    Ui_MainWindow *ui;
};
#endif // MAINWINDOW_H
