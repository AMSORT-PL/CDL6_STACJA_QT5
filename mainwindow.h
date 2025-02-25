#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "phsbackend.h"
#include "layout1.h"
#include <QPushButton>
#include <QSettings>
#include "keyboardnumdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(PHSBackend* _phs, QWidget *parent = nullptr);
    ~MainWindow();
    PHSBackend* phs;

private:
    Ui::MainWindow *ui;
    int alarmId;
    QString name;
    int actualLanguageint;
    bool flagConnectPHS;
    LayoutBase *buttonsWidget;
    KeyboardNumDialog *keyboardNumDialog();

signals:
    void openCont();
    void openReason();
    void showOnTopIfVisible();

private slots:
    void on_ean_button_clicked();
};

#endif // MAINWINDOW_H
