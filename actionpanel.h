#ifndef ACTIONPANEL_H
#define ACTIONPANEL_H

#include <QWidget>

namespace Ui {
class ActionPanel;
}

class ActionPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ActionPanel(QWidget *parent = nullptr);
    ~ActionPanel();

private slots:
    void on_ean_button_clicked();

signals:
    void ean_button_clicked();

private:
    Ui::ActionPanel *ui;
};

#endif // ACTIONPANEL_H
