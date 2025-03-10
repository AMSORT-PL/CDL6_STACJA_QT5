#ifndef LAYOUTBASE_H
#define LAYOUTBASE_H

#include <QWidget>
#include <QPushButton>
#include <QList>

class LayoutBase : public QWidget {
    Q_OBJECT
public:
    explicit LayoutBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~LayoutBase() = default;

    virtual QList<QPushButton*> getButtons() const = 0;

signals:
    // TO MAINWINDOW
    void ean_button_clicked_forward();
    void inventory_tote_clicked_forward();
    void inventory_product_clicked_forward();
    void mark_KJ_clicked_forward();
    void close_carrier_clicked_forward();
    void reprint_clicked_forward();
    void run_receiving_container_clicked_forward();
    void inventory_src_container_clicked_forward();
    void askToMarkLocation(int locationId);

    // TO ACTIONPANEL
    void buttonsUnlocked(bool state);

public slots:
    virtual void markLocation(int locationId) = 0;
    virtual void unmarkLocation() = 0;
};

#endif // LAYOUTBASE_H
