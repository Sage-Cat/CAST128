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
    QByteArray temp {};
    QDataStream in(&temp, QIODevice::WriteOnly);
    in << gen.generate();
    ui->line_key->setText(QString::fromUtf8(temp.toHex()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_encrypt_clicked()
{
    auto keyStr = QByteArray::fromHex(ui->line_key->text().toUtf8());
    auto msgStr = ui->line_msg->text().toUtf8();

    // Addition to 4 (to work with any size of msg)
    while (msgStr.size() % 4 != 0)
        msgStr.push_back('\0');

    // Create key
    CAST128::Key key {};
    {
        QDataStream in(keyStr);
        for (int i = 0; i < CAST128::KEY_LEN; ++i)
            in >> key[i];
    }

    // Create message & encrypt
    QByteArray encrypted {};
    {
        QDataStream res(&encrypted, QIODevice::WriteOnly);
        CAST128 cast128;
        QDataStream in(msgStr);
        while (!in.atEnd())
        {
            CAST128::Message part {};

            for (int i = 0; i < CAST128::MSG_LEN; ++i)
                in >> part[i];

            cast128.encrypt(key, part);

            for (int i = 0; i < CAST128::MSG_LEN; ++i)
                res << part[i];
        }
    }

    ui->line_encrypted->setText(QString::fromUtf8(encrypted.toHex()));
}

void MainWindow::on_btn_decrypt_clicked()
{
    auto encryptedStr = QByteArray::fromHex(ui->line_encrypted->text().toUtf8());
    auto keyStr = QByteArray::fromHex(ui->line_key->text().toUtf8());

    // Create key
    CAST128::Key key {};
    {
        QDataStream in(keyStr);
        for (int i = 0; i < CAST128::KEY_LEN; ++i)
            in >> key[i];
    }

    // Create message & decrypt
    QByteArray decrypted {};
    {
        QDataStream res(&decrypted, QIODevice::WriteOnly);
        CAST128 cast128;
        QDataStream in(encryptedStr);
        while (!in.atEnd())
        {
            CAST128::Message part {};

            for (int i = 0; i < CAST128::MSG_LEN; ++i)
                in >> part[i];

            cast128.decrypt(key, part);

            for (int i = 0; i < CAST128::MSG_LEN; ++i)
                res << part[i];
        }
    }

    // delete \0 at end
    while (decrypted.back() == '\0')
        decrypted.remove(decrypted.size() - 1, 1);

    ui->line_decrypted->setText(QString::fromUtf8(decrypted));
}
