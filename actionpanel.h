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
    void on_inventory_tote_clicked();

    void on_inventory_product_clicked();

    void on_mark_KJ_clicked();

    void on_close_carrier_clicked();

    void on_reprint_clicked();

    void on_run_receiving_container_clicked();

    void on_inventory_src_container_clicked();

signals:
    void ean_button_clicked();
    void inventory_tote_clicked();
    void inventory_product_clicked();
    void mark_KJ_clicked();
    void close_carrier_clicked();
    void reprint_clicked();
    void run_receiving_container_clicked();
    void inventory_src_container_clicked();

public slots:
    void buttonsUnlocked(bool state);

private:
    Ui::ActionPanel *ui;
};

#endif // ACTIONPANEL_H
