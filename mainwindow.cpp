#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QProcess"
#include "QDate"
#include "QTime"
#include "constants.h"
#include "fontmanager.h"

MainWindow::MainWindow(PHSBackend* _phs, QWidget *parent) :
    QMainWindow(parent)
  , phs(_phs)
  , ui(new Ui::MainWindow)
  , buttonsWidget(nullptr)
{
    ui->setupUi(this);


    buttonsWidget = new Layout1(phs, this);


    QVBoxLayout *layout = new QVBoxLayout(ui->buttonFrame);
    layout->addWidget(buttonsWidget);
    ui->buttonFrame->setLayout(layout);

    flagConnectPHS = true;
    QTimer *updateStatusConnectionToPHS = new QTimer(this);
    connect(updateStatusConnectionToPHS, SIGNAL(timeout()), this, SLOT(checkConnectionWithPHS()));
    updateStatusConnectionToPHS->start(500);

    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t, &QTimer::timeout, [&]() {
    emit showOnTopIfVisible();

    } );
    t->start();

    connect(buttonsWidget, &LayoutBase::ean_button_clicked_forward, this, &MainWindow::on_ean_button_clicked);
}

void MainWindow::on_ean_button_clicked() {

}

MainWindow::~MainWindow()
{
    delete ui;
}

