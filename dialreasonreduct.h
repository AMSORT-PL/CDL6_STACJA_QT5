#ifndef DIALREASONREDUCT_H
#define DIALREASONREDUCT_H

#include <QDialog>
#include <QMouseEvent>
#include <QScreen>
#include "phsbackend.h"

namespace Ui {
class DialReasonReduct;
}

class DialReasonReduct : public QDialog
{
    Q_OBJECT

public:
    explicit DialReasonReduct(PHSBackend* _phs, QWidget *parent = 0);
    PHSBackend* phs;
    ~DialReasonReduct();

private slots:
    void on_brakArt_clicked();

    void on_brakMiej_clicked();

    void on_kJ_clicked();

    void on_niepelneOp_clicked();

    void on_brakOW_clicked();

    void on_ajent_clicked();

    void on_zmianaSerii_clicked();

    void on_zamknij_clicked();

    void showwindow();

    void setDarkMode();
    void setLightMode();

public slots:
    void phsConnectionStatusChanged(bool);

private:
    Ui::DialReasonReduct *ui;
    void checkForDarkMode();
    void centerWindow();
    bool dragging;
    QPoint dragStartPosition;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // DIALREASONREDUCT_H
