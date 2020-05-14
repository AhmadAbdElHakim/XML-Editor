#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OpenFileButton_clicked();

    void on_Prettify_Button_clicked();

    void on_Save_Button_clicked();

    void on_check_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
