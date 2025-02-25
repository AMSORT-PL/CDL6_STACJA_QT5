#include "dialreasonreduct.h"
#include "ui_dialreasonreduct.h"

DialReasonReduct::DialReasonReduct(PHSBackend* _phs, QWidget *parent) :
    QDialog(parent),
    phs(_phs),
    dragging(false),
    ui(new Ui::DialReasonReduct)
{
    ui->setupUi(this);

    phs->registerObject(21, ui->brakArt);
    phs->registerObject(22, ui->brakMiej);
    phs->registerObject(23, ui->kJ);
    phs->registerObject(24, ui->niepelneOp);
    phs->registerObject(25, ui->brakOW);
    phs->registerObject(26, ui->ajent);
    phs->registerObject(27, ui->zmianaSerii);
    phs->registerObject(28, ui->zamknij);
}

DialReasonReduct::~DialReasonReduct()
{
    delete ui;
}

void DialReasonReduct::on_brakArt_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {1} );
}

void DialReasonReduct::on_brakMiej_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {2} );
}

void DialReasonReduct::on_kJ_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {3} );
}

void DialReasonReduct::on_niepelneOp_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {4} );
}

void DialReasonReduct::on_brakOW_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {5} );
}

void DialReasonReduct::on_ajent_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {6} );
}

void DialReasonReduct::on_zmianaSerii_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {7} );
}

void DialReasonReduct::on_zamknij_clicked()
{
    phs->callPhsProc(103, QVector<QVariant> {8} );
}

void DialReasonReduct::showwindow()
{
    if(this->isVisible())
    {
        this->hide();
        this->show();
    }
    else
    {
        this->show();
    }
}

void DialReasonReduct::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DialReasonReduct::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && event->buttons() & Qt::LeftButton) {
        QPoint newPosition = event->globalPos() - dragStartPosition;

        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        int x = std::max(screenGeometry.left(), std::min(newPosition.x(), screenGeometry.right() - width()));
        int y = std::max(screenGeometry.top(), std::min(newPosition.y(), screenGeometry.bottom() - height()));

        move(x, y);
        event->accept();
    }
}

void DialReasonReduct::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

void DialReasonReduct::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height() - 15) / 2;

    this->move(x, y);
}

void DialReasonReduct::setDarkMode() {
    ui->ajent->setStyleSheet("background-color: #3E4339;color: white;");
    ui->brakArt->setStyleSheet("background-color: #3E4339;color: white;");
    ui->brakMiej->setStyleSheet("background-color: #3E4339;color: white;");
    ui->brakOW->setStyleSheet("background-color: #3E4339;color: white;");
    ui->kJ->setStyleSheet("background-color: #3E4339;color: white;");
    ui->niepelneOp->setStyleSheet("background-color: #3E4339;color: white;");
    ui->zmianaSerii->setStyleSheet("background-color: #3E4339;color: white;");
    ui->zamknij->setStyleSheet("background-color: #3E4339;color: white;");
    ui->frame->setStyleSheet("QFrame {border: 2px solid #454A40;"
                             "border-radius: 3px;"
                             "background-color: #2D3029;}");
}

void DialReasonReduct::setLightMode() {
    ui->ajent->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->brakArt->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->brakMiej->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->brakOW->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->kJ->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->niepelneOp->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->zmianaSerii->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->zamknij->setStyleSheet("background-color: #E8F7E4;color: black;");
    ui->frame->setStyleSheet("QFrame {border: 2px solid #CCD0C8;"
                             "border-radius: 3px;"
                             "background-color: #F5FBF4;}");
}

void DialReasonReduct::phsConnectionStatusChanged(bool status)
{
    if(!status)
        this->setVisible(false);
}
