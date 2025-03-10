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

    connect(phs, &PHSBackend::noConnection, this, &MainWindow::onDisconnected);
    connect(phs, &PHSBackend::connected, this, &MainWindow::onConnected);


    connect(buttonsWidget, &LayoutBase::ean_button_clicked_forward, this, &MainWindow::on_ean_button_clicked);
    connect(buttonsWidget, &LayoutBase::askToMarkLocation, phs, &PHSBackend::markLocationRequest);
    connect(buttonsWidget, &LayoutBase::inventory_tote_clicked_forward, phs, &PHSBackend::inventoryToteRequest);
    connect(buttonsWidget, &LayoutBase::inventory_product_clicked_forward, phs, &PHSBackend::inventoryProductRequest);
    connect(buttonsWidget, &LayoutBase::mark_KJ_clicked_forward, phs, &PHSBackend::markKJRequest);
    connect(buttonsWidget, &LayoutBase::close_carrier_clicked_forward, phs, &PHSBackend::closeCarrierRequest);
    connect(buttonsWidget, &LayoutBase::reprint_clicked_forward, phs, &PHSBackend::reprintRequest);
    connect(buttonsWidget, &LayoutBase::run_receiving_container_clicked_forward, phs, &PHSBackend::runReceivingContainerRequest);
    connect(buttonsWidget, &LayoutBase::inventory_src_container_clicked_forward, phs, &PHSBackend::inventorySrcContainerRequest);

    /*
    void inventory_button_clicked_forward();
    void inventory_product_clicked_forward();
    void mark_KJ_clicked_forward();
    void close_carrier_clicked_forward();
    void reprint_clicked_forward();
    void run_receiving_container_clicked_forward();
    void inventory_src_container_clicked_forward();
     */


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

void MainWindow::onConnected() {
    ui->conn_label->setText("Połączono");
}

void MainWindow::onDisconnected() {
    ui->conn_label->setText("Brak połączenia");
}

MainWindow::~MainWindow()
{
    delete ui;
}

