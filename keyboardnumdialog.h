#ifndef KEYBOARDNUMDIALOG_H
#define KEYBOARDNUMDIALOG_H

#include <QDialog>
#include "phsbackend.h"

namespace Ui {
class KeyboardNumDialog;
}

class KeyboardNumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyboardNumDialog(PHSBackend* _phsBackend, QWidget *parent = 0);
    ~KeyboardNumDialog();

    void defineLanguage(QStringList const& strings);
public slots:
    void displayStart();
    void displayEnd();

signals:
    void gotEanToSend(QString ean);

private slots:
    void on_confirmPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_bckspcPushButton_clicked();

    void on_d0PushButton_clicked();

    void on_d1PushButton_clicked();

    void on_d2PushButton_clicked();

    void on_d3PushButton_clicked();

    void on_d4PushButton_clicked();

    void on_d5PushButton_clicked();

    void on_d6PushButton_clicked();

    void on_d7PushButton_clicked();

    void on_d8PushButton_clicked();

    void on_d9PushButton_clicked();

private:
    Ui::KeyboardNumDialog *ui;
    PHSBackend *phsBackend;
    void appendChar(QChar digit);
};

#endif // KEYBOARDNUMDIALOG_H
