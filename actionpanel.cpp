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

void ActionPanel::on_inventory_tote_clicked()
{
    emit inventory_tote_clicked();
}


void ActionPanel::on_inventory_product_clicked()
{
    emit inventory_product_clicked();
}


void ActionPanel::on_mark_KJ_clicked()
{
    emit mark_KJ_clicked();
}


void ActionPanel::on_close_carrier_clicked()
{
    emit close_carrier_clicked();
}


void ActionPanel::on_reprint_clicked()
{
    emit reprint_clicked();
}


void ActionPanel::on_run_receiving_container_clicked()
{
    emit run_receiving_container_clicked();
}


void ActionPanel::on_inventory_src_container_clicked()
{
    emit inventory_src_container_clicked();
}

void ActionPanel::buttonsUnlocked(bool state) {
    ui->close_carrier->setEnabled(state);
    ui->run_receiving_container->setEnabled(state);
    ui->mark_KJ->setEnabled(state);
    ui->inventory_tote->setEnabled(state);
    ui->reprint->setEnabled(state);
    ui->inventory_src_container->setEnabled(state);
    ui->edit_last->setEnabled(state);
}

