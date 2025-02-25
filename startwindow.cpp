#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(PHSBackend* _phs, QWidget *parent) :
    QDialog(parent),
    phs(_phs),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    phs->registerObject(8, ui->komunikaty);

    QFont boldFont = FontManager::instance().getMontserratBold();

    boldFont.setPointSize(20);
    boldFont.setBold(true);

    ui->label_3->setFont(boldFont);

    QFont normalFont = FontManager::instance().getMontserratMedium();
    normalFont.setPointSize(16);

    ui->label->setFont(normalFont);
    ui->statusPhs->setFont(normalFont);

    normalFont.setPointSize(14);

    ui->komunikaty->setFont(normalFont);

}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::phsConnectionStatusChanged(bool status)
{
    ui->statusPhs->setText(status ? "Połączony" : "Rozłączony");
    ui->statusPhs->setStyleSheet(status ? "background : green" : "background : red");
}
