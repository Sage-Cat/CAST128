#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRandomGenerator>

#include <QByteArray>
#include <QtEndian>
#include <QIODevice>

#include "cast128.h"

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // TEST
    {
        // #1
        CAST128 cast128;
        static const CAST128::Key KEY = { 0x01234567, 0x12345678, 0x23456789, 0x3456789A };
        CAST128::Message msg = { 0x01234567, 0x89ABCDEF };

        cast128.encrypt( KEY, msg );
        Q_ASSERT(msg[0] == 0x238B4FE5);

        cast128.decrypt( KEY, msg );
        Q_ASSERT(msg[0] == 0x01234567);
    }

    QRandomGenerator gen;
    ui->line_key->setText(QString::fromUtf8(QByteArray::number(gen.generate64()).toHex()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_encrypt_clicked()
{
    auto keyStr = QByteArray::fromHex(ui->line_key->text().toUtf8());
    auto msgStr = ui->line_msg->text().toUtf8();

    msgStr = QByteArray::fromHex("01234567");

    // Create key
    CAST128::Key key {};
    QDataStream in(keyStr);
    for (int i = 0; i < CAST128::KEY_LEN; ++i)
        in >> key[i];

    // Create message & encrypt
    QByteArray encrypted {};
    QDataStream res(&encrypted, QIODevice::ReadWrite);
    CAST128 cast128;
    in << msgStr;
    while (!in.atEnd())
    {
        CAST128::Message part {};

        for (int i = 0; i < CAST128::MSG_LEN; ++i)
            in >> key[i - 1];

        cast128.encrypt(key, part);
    }

    ui->line_encrypted->setText(QString::fromUtf8(encrypted.toHex()));
}

void MainWindow::on_btn_decrypt_clicked()
{
    auto encrypted = QByteArray::fromHex(ui->line_encrypted->text().toUtf8());
    auto key = QByteArray::fromHex(ui->line_key->text().toUtf8());


    ui->line_decrypted->setText(QString::fromUtf8(""));
}
