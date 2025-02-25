#ifndef LAYOUT1_H
#define LAYOUT1_H

#include <QMainWindow>
#include "phsbackend.h"
#include "ui_layout1.h"
#include <layoutbase.h>
#include <QPushButton>
#include <QSettings>
#include <QApplication>

QT_BEGIN_NAMESPACE
namespace Ui {
class Layout1;
}

QT_END_NAMESPACE

class Layout1 : public LayoutBase
{
    Q_OBJECT

public:
    Layout1(PHSBackend* _phs, QWidget *parent = nullptr);
    ~Layout1();
    QList<QPushButton*> getButtons() const override;

private:
    Ui::Layout1 *ui;
    PHSBackend* phs;
    QList<QPushButton*> buttons;
    void assignButtons();
    void connectButtons();
    QString name;

private slots:
    void buttonClicked();
};

#endif // LAYOUT1_H
