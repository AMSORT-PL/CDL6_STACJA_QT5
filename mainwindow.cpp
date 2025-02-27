#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    connect(buttonsWidget, &LayoutBase::ean_button_clicked_forward, this, &MainWindow::on_ean_button_clicked);
    connect(buttonsWidget, &LayoutBase::askToMarkLocation, phs, &PHSBackend::markLocationRequest);


    connect(phs, &PHSBackend::markLocation, buttonsWidget, &LayoutBase::markLocation);
    connect(keyboardNumDialog, &KeyboardNumDialog::gotEanToSend, phs, &PHSBackend::sendEan);
}

void MainWindow::on_ean_button_clicked() {
    keyboardNumDialog->open();

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    QSize dialogSize = keyboardNumDialog->size();

    int x = screenGeometry.x() + (screenGeometry.width() - dialogSize.width()) / 2;
    int y = screenGeometry.y() + 50 + (screenGeometry.height() - dialogSize.height()) / 2;

    keyboardNumDialog->move(x, y);
}

MainWindow::~MainWindow()
{
    delete ui;
}

