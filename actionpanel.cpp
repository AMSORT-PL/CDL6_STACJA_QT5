#include "actionpanel.h"
#include "ui_actionpanel.h"
#include <QDebug>
#include <QSvgWidget>

ActionPanel::ActionPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActionPanel)
{
    ui->setupUi(this);

    QSvgWidget *svg = new QSvgWidget(this);
    svg->load(QString(":/worker.svg"));
    svg->setStyleSheet("background-color: transparent");
    svg->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->gridLayout_2->addWidget(svg);

}

ActionPanel::~ActionPanel()
{
    delete ui;
}

void ActionPanel::on_ean_button_clicked()
{
    emit ean_button_clicked();
}

