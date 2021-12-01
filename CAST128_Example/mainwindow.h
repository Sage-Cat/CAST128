#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static const size_t BYTE_SIZE = 8; // in bits

private slots:
    void on_btn_encrypt_clicked();
    void on_btn_decrypt_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
