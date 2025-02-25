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

private:
    Ui::ActionPanel *ui;
};

#endif // ACTIONPANEL_H
