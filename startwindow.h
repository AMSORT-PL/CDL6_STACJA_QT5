#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include "phsbackend.h"
#include "fontmanager.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(PHSBackend* _phs, QWidget *parent = 0);
    PHSBackend* phs;
    ~StartWindow();

public slots:
    void phsConnectionStatusChanged(bool);

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
