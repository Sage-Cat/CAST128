#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRandomGenerator>

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QMessageBox>
#include <QRegularExpression>

#include "cast128.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QByteArray temp {};
    QDataStream out(&temp, QIODevice::WriteOnly);
    for (int i = 0; i < CAST128::KEY_LEN; ++i)
        out << QRandomGenerator::system()->generate();
    ui->line_key->setText(QString::fromUtf8(temp.toHex()));
    ui->line_key->setMaxLength(CAST128::KEY_LEN * 8);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_encrypt_clicked()
{
    const QString keyText = ui->line_key->text();
    static const QRegularExpression hexKey(QStringLiteral("^[0-9A-Fa-f]{32}$"));
    if (!hexKey.match(keyText).hasMatch()) {
        QMessageBox::warning(this, tr("Invalid key"), tr("Enter exactly 32 hexadecimal key characters."));
        return;
    }

    auto keyStr = QByteArray::fromHex(keyText.toUtf8());
    auto msgStr = ui->line_msg->text().toUtf8();

    // Zero-pad to CAST-128's eight-byte block size.
    while (msgStr.size() % 8 != 0)
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
    const QString keyText = ui->line_key->text();
    const QString encryptedText = ui->line_encrypted->text();
    static const QRegularExpression hexKey(QStringLiteral("^[0-9A-Fa-f]{32}$"));
    static const QRegularExpression hexData(QStringLiteral("^(?:[0-9A-Fa-f]{16})*$"));
    if (!hexKey.match(keyText).hasMatch()) {
        QMessageBox::warning(this, tr("Invalid key"), tr("Enter exactly 32 hexadecimal key characters."));
        return;
    }
    if (!hexData.match(encryptedText).hasMatch()) {
        QMessageBox::warning(this, tr("Invalid ciphertext"), tr("Enter hexadecimal data in complete 8-byte blocks (16 characters each)."));
        return;
    }

    auto encryptedStr = QByteArray::fromHex(encryptedText.toUtf8());
    auto keyStr = QByteArray::fromHex(keyText.toUtf8());

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
    while (!decrypted.isEmpty() && decrypted.back() == '\0')
        decrypted.remove(decrypted.size() - 1, 1);

    ui->line_decrypted->setText(QString::fromUtf8(decrypted));
}
