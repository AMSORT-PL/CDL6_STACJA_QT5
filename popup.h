#ifndef POPUP_H
#define POPUP_H
#include <QObject>
#include <QDialog>
#include <QMouseEvent>
#include <QScreen>
#include "phsbackend.h"

namespace Ui {
class PopUp;
}

class PopUp : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(int alarmId READ getAlarmId WRITE setAlarmId)

public:
    explicit PopUp(PHSBackend* _phs, QWidget *parent = nullptr);
    ~PopUp();
    PHSBackend* phs;


private:
    Ui::PopUp *ui;
    int _alarmId;
    void setAlarmId(int alarm);
    void centerWindow();
    bool dragging;
    QPoint dragStartPosition;
    inline int getAlarmId() const { return _alarmId; }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    void dataFromPhs( QVector<QVariant> const& params);
    void phsConnectionStatusChanged(bool);
private slots:
    void on_zatwierdz_clicked();
    void on_anuluj_clicked();
    void setOnTopIfVisible();
    void setDarkMode();
    void setLightMode();
};

#endif // POPUP_H


