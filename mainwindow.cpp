#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QProcess"
#include "QDate"
#include "QTime"
#include "constants.h"
#include "fontmanager.h"
#include <QScreen>

MainWindow::MainWindow(PHSBackend* _phs, QWidget *parent) :
    QMainWindow(parent),
    phs(_phs),
    ui(new Ui::MainWindow),
    buttonsWidget(nullptr),
    keyboardNumDialog(new KeyboardNumDialog(phs))

{
    ui->setupUi(this);


    buttonsWidget = new Layout1(phs, this);


    QVBoxLayout *layout = new QVBoxLayout(ui->buttonFrame);
    layout->addWidget(buttonsWidget);
    layout->setMargin(6);
    ui->buttonFrame->setLayout(layout);

    keyboardNumDialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog
                               | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    keyboardNumDialog->setAttribute(Qt::WA_TranslucentBackground);


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
    keyboardNumDialog->open();

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    // Pobranie rozmiaru okna dialogowego
    QSize dialogSize = keyboardNumDialog->size();

    // Obliczenie pozycji środkowej
    int x = screenGeometry.x() + (screenGeometry.width() - dialogSize.width()) / 2;
    int y = screenGeometry.y() + (screenGeometry.height() - dialogSize.height()) / 2;

    // Ustawienie pozycji
    keyboardNumDialog->move(x, y);
}

MainWindow::~MainWindow()
{
    delete ui;
}

