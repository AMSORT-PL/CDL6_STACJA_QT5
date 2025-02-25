#include "actionpanel.h"
#include "ui_actionpanel.h"

ActionPanel::ActionPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActionPanel)
{
    ui->setupUi(this);
}

ActionPanel::~ActionPanel()
{
    delete ui;
}
