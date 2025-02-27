#include "keyboardnumdialog.h"
#include "ui_keyboardnumdialog.h"

#include "constants.h"

KeyboardNumDialog::KeyboardNumDialog(PHSBackend *_phsBackend, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyboardNumDialog),
    phsBackend(_phsBackend),
    errorTimer(new QTimer())
{
    ui->setupUi(this);

    int l = 0;
    int r = 0;
    int t = 0;
    int b = 0;

    ui->eanLineEdit->getTextMargins(&l, &t, &r, &b);

    ui->eanLineEdit->setTextMargins(l + 5, t, r, b);

    errorTimer->setInterval(2000);
    connect(errorTimer, SIGNAL(timeout()), this, SLOT(clearErrorMessage()));
}

KeyboardNumDialog::~KeyboardNumDialog()
{
    delete ui;
}

void KeyboardNumDialog::displayStart()
{
    ui->eanLineEdit->clear();

    show();
}

void KeyboardNumDialog::displayEnd()
{
    ui->eanLineEdit->clear();

    hide();
}

void KeyboardNumDialog::on_confirmPushButton_clicked()
{
    QString text = QString::fromStdString("]E0");
    text.append(ui->eanLineEdit->text());

    if(ui->eanLineEdit->text() < 1) {
        setErrorMessage("Nie można przesłać pustego pola");
        return;   
    }

    displayEnd();

    emit gotEanToSend(text);
}

void KeyboardNumDialog::on_cancelPushButton_clicked()
{
    displayEnd();
}

void KeyboardNumDialog::on_bckspcPushButton_clicked()
{
    QString text = ui->eanLineEdit->text();

    if(text.size() < 1)
        return;

    text.resize(text.size() - 1);

    ui->eanLineEdit->setText(text);
}

void KeyboardNumDialog::setErrorMessage(QString message) {
    ui->error_label->setText(message);
    errorTimer->start();
}

void KeyboardNumDialog::clearErrorMessage() {
    ui->error_label->setText("");
}

void KeyboardNumDialog::on_d0PushButton_clicked()
{
    appendChar('0');
}

void KeyboardNumDialog::on_d1PushButton_clicked()
{
    appendChar('1');
}

void KeyboardNumDialog::on_d2PushButton_clicked()
{
    appendChar('2');
}

void KeyboardNumDialog::on_d3PushButton_clicked()
{
    appendChar('3');
}

void KeyboardNumDialog::on_d4PushButton_clicked()
{
    appendChar('4');
}

void KeyboardNumDialog::on_d5PushButton_clicked()
{
    appendChar('5');
}

void KeyboardNumDialog::on_d6PushButton_clicked()
{
    appendChar('6');
}

void KeyboardNumDialog::on_d7PushButton_clicked()
{
    appendChar('7');
}

void KeyboardNumDialog::on_d8PushButton_clicked()
{
    appendChar('8');
}

void KeyboardNumDialog::on_d9PushButton_clicked()
{
    appendChar('9');
}

void KeyboardNumDialog::appendChar(QChar digit)
{
    if(ui->eanLineEdit->text().size() >= ME::EAN_INP_MAX_LENGTH)
        return;

    QString text = ui->eanLineEdit->text();
    text.append(digit);
    ui->eanLineEdit->setText(text);
}

void KeyboardNumDialog::defineLanguage(QStringList const& strings)
{
    ui->label->setText(strings[3]);
    ui->cancelPushButton->setText(strings[2]);
    ui->confirmPushButton->setText(strings[1]);
}

