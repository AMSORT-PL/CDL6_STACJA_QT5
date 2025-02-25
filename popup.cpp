#include "popup.h"
#include "ui_popup.h"

PopUp::PopUp(PHSBackend* _phs, QWidget *parent) :
    QDialog(parent),
    phs(_phs),
    ui(new Ui::PopUp),
    dragging(false),
    _alarmId(0)
{
    ui->setupUi(this);
    phs->registerObject(13, ui->komunikat);
    phs->registerObject(20, ui->zatwierdz);
    phs->registerObject(19, ui->anuluj);
}

PopUp::~PopUp()
{
    delete ui;
    phs->registerProc(90, new PHSClosureWrapper<PopUp>(this, &PopUp::dataFromPhs));
}

void PopUp::dataFromPhs(const QVector<QVariant> &params)
{
    qInfo() << "paramsy = " + params[0].toString();
    // alarmIdik = params[0].toInt();
}

void PopUp::on_zatwierdz_clicked()
{
    phs->callPhsProc(90, QVector<QVariant> { _alarmId, 0} );
    centerWindow();
}

void PopUp::on_anuluj_clicked()
{
    phs->callPhsProc(90, QVector<QVariant> { _alarmId, 1} );
    centerWindow();
}

void PopUp::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height() - 15) / 2;

    this->move(x, y);
}

void PopUp::setOnTopIfVisible()
{

    if(this->isVisible())
    {
        this->activateWindow();
        this->raise();
    }

}

void PopUp::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void PopUp::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && event->buttons() & Qt::LeftButton) {
        QPoint newPosition = event->globalPos() - dragStartPosition;

        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        int x = std::max(screenGeometry.left(), std::min(newPosition.x(), screenGeometry.right() - width()));
        int y = std::max(screenGeometry.top(), std::min(newPosition.y(), screenGeometry.bottom() - height()));

        move(x, y);
        event->accept();
    }
}

void PopUp::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

void PopUp::setDarkMode() {
    ui->anuluj->setStyleSheet("color: white; background-color: #3E4339;padding-left: 10px;padding-right: 10px;");
    ui->zatwierdz->setStyleSheet("color: white; background-color: #3E4339;padding-left: 10px;padding-right: 10px;");
    ui->komunikat->setStyleSheet("color: white; border: none;");
    ui->frame->setStyleSheet("QFrame {\nborder: 2px solid #454A40;\nborder-radius: 3px;\nbackground-color: #2D3029;\n}");
}

void PopUp::setLightMode() {
    ui->anuluj->setStyleSheet("color: black; background-color: #E8F7E4;padding-left: 10px;padding-right: 10px;");
    ui->zatwierdz->setStyleSheet("color: black; background-color: #E8F7E4;padding-left: 10px;padding-right: 10px;");
    ui->komunikat->setStyleSheet("color: black; border: none;");
    ui->frame->setStyleSheet("QFrame {\nborder: 2px solid #CCD0C8;\nborder-radius: 3px;\nbackground-color: #F5FBF4;\n}");
}

void PopUp::setAlarmId(int alarm)
{
    if (_alarmId == alarm)
        return;
    _alarmId = alarm;
    if( _alarmId > 0) this->show();
    else this->hide();
}


void PopUp::phsConnectionStatusChanged(bool status)
{
    if(!status)
        this->setVisible(false);
}
