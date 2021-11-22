#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "cast128.h"

using namespace std;

static string outFileKey { "out.txt" };
static string inFileKey { "input.txt" };
static string keyFileKey { "key.txt" };

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Cast128::generateKey(outFileKey);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_encrypt_clicked()
{
    {
        ofstream toInFile(inFileKey);
        toInFile << ui->textEdit->toPlainText().toStdString();
        toInFile.close();
    }

    try {
        Cast128::Key key;
        Cast128::readKey(keyFileKey, &key);
        Cast128::encryptFile(inFileKey, outFileKey, key);

        {
            ifstream fromOutFile(outFileKey);
            string text {};
            fromOutFile >> text;
            ui->textEdit->setText(QString::fromStdString(text));
            fromOutFile.close();
        }

    } catch (exception &e) {
        std::cout << e.what();
    }
}

void MainWindow::on_btn_decrypt_clicked()
{
    try {
        Cast128::Key key;
        Cast128::readKey(keyFileKey, &key);
        Cast128::decryptFile(outFileKey, outFileKey, key);

        {
            ifstream fromOutFile(outFileKey);
            string text {};
            fromOutFile >> text;
            ui->textEdit->setText(QString::fromStdString(text));
            fromOutFile.close();
        }

    } catch (exception &e) {
        std::cout << e.what();
    }
}
