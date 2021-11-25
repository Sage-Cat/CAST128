#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRandomGenerator>

#include <cstdint>
#include <fstream>
#include <iostream>
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

    QRandomGenerator gen;

    ui->line_key->setText(QString::number(gen.generate64()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_encrypt_clicked()
{
    {
        ofstream out(inFileKey);
        out << ui->line_msg->text().toStdString();
        out.close();
    }

    {
        ofstream out(keyFileKey);
        out << ui->line_key->text().toStdString();
        out.close();
    }

    try {
        Cast128::Key key;
        Cast128::readKey(keyFileKey, &key);
        Cast128::encryptFile(inFileKey, outFileKey, key);

        {
            ifstream fromOutFile(outFileKey);
            string text {};
            fromOutFile >> text;
            ui->line_encrypted->setText(QString::fromUtf8(QByteArray::fromStdString(text).toHex()));
            fromOutFile.close();
        }

    } catch (exception& e) {
        std::cout << e.what();
    }
}

void MainWindow::on_btn_decrypt_clicked()
{
    {
        ofstream out(inFileKey);
        out << QByteArray::fromHex(ui->line_encrypted->text().toUtf8()).toStdString();
        out.close();
    }

    {
        ofstream out(keyFileKey);
        out << ui->line_key->text().toStdString();
        out.close();
    }

    try {
        Cast128::Key key;
        Cast128::readKey(keyFileKey, &key);
        Cast128::decryptFile(inFileKey, outFileKey, key);

        {
            ifstream fromOutFile(outFileKey);
            string text {};
            fromOutFile >> text;
            ui->line_decrypted->setText(QString::fromStdString(text));
            fromOutFile.close();
        }

    } catch (exception& e) {
        std::cout << e.what();
    }
}
